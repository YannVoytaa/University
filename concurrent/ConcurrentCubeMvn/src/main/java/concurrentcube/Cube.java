package concurrentcube;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.BiConsumer;

public class Cube {
    private final static int WALLS_NUMBER = 6;
    private final static int REPRESENTATIVES_NUMBER = WALLS_NUMBER / 2;
    private final static int GROUPS_NUMBER = REPRESENTATIVES_NUMBER + 1; //interpreting 'show' as a last group

    private final int size;
    private final BiConsumer<Integer, Integer> beforeRotation;
    private final BiConsumer<Integer, Integer> afterRotation;
    private final Runnable beforeShowing;
    private final Runnable afterShowing;
    private final Semaphore[] semaphores = new Semaphore[GROUPS_NUMBER];
    private final AtomicInteger[] isWaiting = new AtomicInteger[GROUPS_NUMBER]; //atomicInteger, because interrupted threads can modify it at the same time
    private final Semaphore representatives = new Semaphore(0);
    private final Semaphore mutex = new Semaphore(1);
    private final Wall[] walls = new Wall[WALLS_NUMBER];
    private int workingSide = -1;
    private final AtomicInteger representativesWaiting = new AtomicInteger(0); //interrupted representatives can modify it at the same time as non-interrupted ones
    private final AtomicBoolean[] representativeInterrupted = new AtomicBoolean[GROUPS_NUMBER];
    private boolean workingSideInProgress = false;
    private final Semaphore[] layerSemaphores;
    private int isWorking = 0;
    private final Semaphore mutexForInterrupted = new Semaphore(1);

    public Cube(int size, BiConsumer<Integer, Integer> beforeRotation, BiConsumer<Integer, Integer> afterRotation, Runnable beforeShowing, Runnable afterShowing) {
        this.size = size;
        this.beforeRotation = beforeRotation;
        this.afterRotation = afterRotation;
        this.beforeShowing = beforeShowing;
        this.afterShowing = afterShowing;
        layerSemaphores = new Semaphore[size];
        for (int i = 0; i < size; i++) {
            layerSemaphores[i] = new Semaphore(1);
        }
        for (int i = 0; i < GROUPS_NUMBER; i++) {
            semaphores[i] = new Semaphore(0);
            isWaiting[i] = new AtomicInteger(0);
            representativeInterrupted[i] = new AtomicBoolean(false);
        }
        for (int i = 0; i < WALLS_NUMBER; i++) {
            walls[i] = new Wall(i);
        }
        walls[0].setNeighbors(walls[1], walls[4], walls[3], walls[2], this::transformToUpperEdge, this::transformToUpperEdge, this::transformToUpperEdge, this::transformToUpperEdge);
        walls[1].setNeighbors(walls[4], walls[0], walls[2], walls[5], this::transformToRightEdge, this::transformToLeftEdge, this::transformToLeftEdge, this::transformToLeftEdge);
        walls[2].setNeighbors(walls[1], walls[0], walls[3], walls[5], this::transformToRightEdge, this::transformToDownEdge, this::transformToLeftEdge, this::transformToUpperEdge);
        walls[3].setNeighbors(walls[2], walls[0], walls[4], walls[5], this::transformToRightEdge, this::transformToRightEdge, this::transformToLeftEdge, this::transformToRightEdge);
        walls[4].setNeighbors(walls[3], walls[0], walls[1], walls[5], this::transformToRightEdge, this::transformToUpperEdge, this::transformToLeftEdge, this::transformToDownEdge);
        walls[5].setNeighbors(walls[1], walls[2], walls[3], walls[4], this::transformToDownEdge, this::transformToDownEdge, this::transformToDownEdge, this::transformToDownEdge);
    }

    public void rotate(int side, int layer) throws InterruptedException {
        waitUntilGroupCanRotate(side);
        if(Thread.currentThread().isInterrupted()) {
            signalEndedRotation(side);
            throw new InterruptedException();
        }
        int realLayer = side < 3 ? layer : size - 1 - layer;
        layerSemaphores[realLayer].acquire();
        beforeRotation.accept(side, layer);
        commitRotation(side, layer);
        afterRotation.accept(side, layer);
        layerSemaphores[realLayer].release();
        signalEndedRotation(side);
    }

    public String show() throws InterruptedException {
        int side = WALLS_NUMBER; //interpreting 'show' group as the last group with only one layer
        waitUntilGroupCanRotate(side);
        if(Thread.currentThread().isInterrupted()) {
            signalEndedRotation(side);
            throw new InterruptedException();
        }
        beforeShowing.run();
        String result = getCubeState();
        afterShowing.run();
        signalEndedRotation(side);
        return result;
    }

    private void commitRotation(int side, int layer) {
        Wall curr = walls[side];
        Wall left = curr.leftNeighbor;
        Wall up = curr.upNeighbor;
        Wall right = curr.rightNeighbor;
        Wall down = curr.downNeighbor;

        List<Integer> leftState = left.state;
        List<Integer> upState = up.state;
        List<Integer> rightState = right.state;
        List<Integer> downState = down.state;
        for (int i = 0; i < size; i++) {
            int tmp = leftState.get(curr.leftEdge.getPosition(layer, i));
            leftState.set(curr.leftEdge.getPosition(layer, i), downState.get(curr.downEdge.getPosition(layer, i)));
            downState.set(curr.downEdge.getPosition(layer, i), rightState.get(curr.rightEdge.getPosition(layer, i)));
            rightState.set(curr.rightEdge.getPosition(layer, i), upState.get(curr.upEdge.getPosition(layer, i)));
            upState.set(curr.upEdge.getPosition(layer, i), tmp);
        }
        if (layer == 0) {
            transformRight(side);
        }
        if (layer == size - 1) {
            transformLeft(getOpposite(side));
        }
    }

    private void transform(int side, Transform newPos) {
        List<Integer> state = walls[side].state;
        List<Integer> newState = new ArrayList<>(state);
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                newState.set(newPos.getPosition(row, col), state.get(transformToUpperEdge(row, col)));
            }
        }
        walls[side].state = newState;
    }

    private int transformToLeftEdge(int row, int col) {
        return (size - 1 - col) * size + row;
    }

    private int transformToUpperEdge(int row, int col) {
        return row * size + col;
    }

    private int transformToRightEdge(int row, int col) {
        return col * size + (size - 1 - row);
    }

    private int transformToDownEdge(int row, int col) {
        return (size - 1 - row) * size + size - 1 - col;
    }

    private void transformRight(int side) {
        transform(side, this::transformToRightEdge);
    }

    private void transformLeft(int side) {
        transform(side, this::transformToLeftEdge);
    }

    private String getCubeState() {
        return Arrays.stream(walls)
                .map(wall -> wall.state.stream()
                        .map(Objects::toString)
                        .reduce("", String::concat))
                .reduce("", String::concat);
    }

    private void waitUntilGroupCanRotate(int side) throws InterruptedException {
        mutex.acquire();
        int id = getId(side);
        if (workingSide == id && workingSideInProgress && representativesWaiting.get() == 0) {
            //if no other groups are waiting, we let our rotation go
            //(necessary to pass test 'notBlockingSameSide')
        }
        else if (otherSideTurn(id) || workingSideInProgress) {
            if (isWaiting[id].getAndIncrement() > 0) {
                mutex.release();
                //semaphores[id].acquireUninterruptibly();
                try {
                    semaphores[id].acquire();
                } catch (InterruptedException e) {
                    mutexForInterrupted.acquireUninterruptibly();
                    isWaiting[id].decrementAndGet();
                    if(semaphores[id].tryAcquire()) { //if someone released us, but interruption happened before it
                        if(representativeInterrupted[id].getAndSet(false)) { //interrupted representative released us, but we also got interrupted
                            representativesWaiting.decrementAndGet();
                            if(representatives.tryAcquire()) { //if someone released representative, but interruption happened before it
                                mutex.release();
                            }
                        }
                        else {
                            mutex.release();
                        }
                    }
                    mutexForInterrupted.release();
                    throw new InterruptedException();
                }
            } else {
                representativesWaiting.incrementAndGet();
                mutex.release();
                //representatives.acquireUninterruptibly();
                beRepresentative(id);
                representativesWaiting.decrementAndGet();
                workingSide = id;
                workingSideInProgress = true;
            }
            if (representativeInterrupted[id].getAndSet(false)) {
                beRepresentative(id);
                representativesWaiting.decrementAndGet();
                workingSide = id;
                workingSideInProgress = true;
            }

            isWaiting[id].decrementAndGet();
        }
        else {
            workingSide = id;
            workingSideInProgress = true;
        }
        isWorking++;
        if (isWaiting[id].get() > 0) {
            semaphores[id].release();
        } else {
            mutex.release();
        }
    }

    private void beRepresentative(int id) throws InterruptedException {
        try {
            representatives.acquire();
        } catch (InterruptedException e) {
            mutexForInterrupted.acquireUninterruptibly();
            if (isWaiting[id].decrementAndGet() > 0) {
                representativeInterrupted[id].set(true);
                semaphores[id].release();
            }
            else {
                representativesWaiting.decrementAndGet();
                if(representatives.tryAcquire()) { //if someone released representative, but interruption happened before it
                    mutex.release();
                }
            }
            mutexForInterrupted.release();
            throw new InterruptedException();
        }
    }

    private boolean otherSideTurn(int side) {
        return workingSide != -1 && workingSide != side;
    }

    private void signalEndedRotation(int side) {
        mutex.acquireUninterruptibly(); //quick cleaning; same opertations would have to happen when catching InterruptedException
        if (--isWorking > 0) {
            mutex.release();
            return;
        }
        workingSide = -1;
        workingSideInProgress = false;
        if (representativesWaiting.get() > 0) {
            representatives.release();
        } else {
            mutex.release();
        }
    }

    private int getId(int side) {
        //0 (góra), 1 (lewo), 2 (przód), 3 (prawo), 4 (tył), 5 (dół).
        //id- 0 (lewo/prawo), 1 (przód/tył), 2 (góra/dół)
        if (side == WALLS_NUMBER) {
            //id of 'show' action
            return GROUPS_NUMBER - 1;
        }

        return ((side + 2) % 5) % 3;
    }

    private int getOpposite(int side) {
        //0 (góra), 1 (lewo), 2 (przód), 3 (prawo), 4 (tył), 5 (dół).
        return (side + (side >= 3 ? 1 : -1) + 3) % 3 + (side >= 3 ? 0 : 3);
    }

    private interface Transform {
        int getPosition(int row, int col);
    }

    private class Wall {
        private List<Integer> state = new ArrayList<>();
        private Wall leftNeighbor, upNeighbor, rightNeighbor, downNeighbor;
        private Transform leftEdge, upEdge, rightEdge, downEdge; //information about adjacent cells
        //for example, right edge of left wall is adjacent to left edge of front wall, thus frontWall.leftNeighbor = leftWall &&
        //frontWall.leftEdge = rightEdge && leftWall.rightNeighbour = frontWall && leftWall.rightEdge = leftEdge
        //another example: upperWall.rightEdge is adjacent to rightWall.upperEdge, thus upperWall.rightNeighbour = rightWall &&
        //upperWall.rightEdge = upperEdge && rightWall.upperNeighbour = upperWall && rightWall.upperEdge = rightEdge

        public Wall(int color) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    state.add(color);
                }
            }
        }

        public void setNeighbors(Wall left, Wall up, Wall right, Wall down, Transform leftEdge, Transform upEdge, Transform rightEdge, Transform downEdge) {
            leftNeighbor = left;
            upNeighbor = up;
            rightNeighbor = right;
            downNeighbor = down;
            this.leftEdge = leftEdge;
            this.upEdge = upEdge;
            this.rightEdge = rightEdge;
            this.downEdge = downEdge;
        }
    }
}