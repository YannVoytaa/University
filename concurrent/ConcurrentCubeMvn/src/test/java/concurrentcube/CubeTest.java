package concurrentcube;

import org.junit.jupiter.api.Test;

import java.time.Duration;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.*;

class CubeTest {

    private final String[] statesAfterOneRotation = {
            "0" +
                    "2341" +
                    "5",

            "4" +
                    "1035" +
                    "2",

            "1" +
                    "5204" +
                    "3",

            "2" +
                    "1530" +
                    "4",

            "3" +
                    "0254" +
                    "1",

            "0" +
                    "4123" +
                    "5",

    };

    void emptyRunnable(){}
    void emptyBi(int a, int b){}

    @Test
    void rotate() {
        for (int size = 1; size < 6; size++) {
            Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
            try {
                fullTurns(cube, size);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    void fullTurns(Cube cube, int size) throws InterruptedException {
        String defString = cube.show();
        for (int side = 0; side < 6; side++) {
            for (int fullTurn = 0; fullTurn < 4; fullTurn++) {
                for (int layer = 0; layer < size; layer++) {
                    cube.rotate(side, layer);
                }
                singleColorWalls(cube, size);
            }
            assertEquals(cube.show(), defString);
        }
    }

    void singleColorWalls(Cube cube, int size) throws InterruptedException {
        String res = cube.show();
        for (int wall = 0; wall < 6; wall++) {
            for (int j = wall * size * size; j < (wall + 1) * size * size; j++) {
                assertEquals(res.charAt(wall * size * size), res.charAt(j));
            }
        }
    }

    @Test
    void singleRotations() {
        String defaultState =
                "0" +
                        "1234" +
                        "5";
        Cube cube = new Cube(1, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        for (int side = 0; side < 6; side++) {
            try {
                cube.rotate(side, 0);
                assertEquals(cube.show(), statesAfterOneRotation[side]);
                for (int rotationsToFullTurn = 1; rotationsToFullTurn < 4; rotationsToFullTurn++) {
                    cube.rotate(side, 0);
                }
                assertEquals(cube.show(), defaultState);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    void oppositeRotations() {
        int size = 5;
        int[] opposite = new int[6];
        opposite[0] = 5;
        opposite[1] = 3;
        opposite[2] = 4;
        opposite[3] = 1;
        opposite[4] = 2;
        opposite[5] = 0;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        String defaultState;
        try {
            defaultState = cube.show();
        } catch (InterruptedException e) {
            e.printStackTrace();
            defaultState = "0".repeat(size * size) + "1".repeat(size * size) + "2".repeat(size * size) + "3".repeat(size * size) +
                    "4".repeat(size * size) + "5".repeat(size * size);
        }
        for(int side = 0; side < 6; side++) {
            for(int layer = 0; layer < size; layer++) {
                try {
                    cube.rotate(side, layer);
                    cube.rotate(opposite[side], size - 1 - layer);
                    assertEquals(cube.show(), defaultState);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Test
    void rotateEdgeLayer() {
        int size = 5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        try {
            cube.rotate(2, 0);
            cube.rotate(0, 0);
            assertEquals(cube.show(), "10000".repeat(5) + "2".repeat(5) + "11115".repeat(4) + "03333" + "2".repeat(20) +
                    "4".repeat(5) + "03333".repeat(4) + "11115" + "4".repeat(20) + "3".repeat(5) + "5".repeat(20));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Test
    void rotateEdgeLayerOpposite() {
        int size = 5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        try {
            cube.rotate(2, 0);
            cube.rotate(5, 4);
            assertEquals(cube.show(), "00001".repeat(5) + "4".repeat(5) + "11115".repeat(4) + "11115" + "2".repeat(20) +
                    "2".repeat(5) + "03333".repeat(4) + "03333" + "4".repeat(20) + "3".repeat(5) + "5".repeat(20));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Test
    void notRotatingDifferentSides() {
        @SuppressWarnings("unchecked")
        Set<Integer>[] sets = new Set[6];
        for (int i = 0; i < 6; i++) {
            sets[i] = new HashSet<>();
            sets[i].add(i);
        }
        sets[0].add(5);
        sets[1].add(3);
        sets[2].add(4);
        sets[3].add(1);
        sets[4].add(2);
        sets[5].add(0);
        int[] working = new int[6];
        Map<Integer, Set<Integer>> workedAtTheSameTime = new ConcurrentHashMap<>();
        Semaphore mutex = new Semaphore(1);
        Cube cube = new Cube(5,
                (side, layer) -> {
                    mutex.acquireUninterruptibly();
                    working[side]++;
                    for (int side2 = 0; side2 < 6; side2++) {
                        if (working[side2] > 0) {
                            workedAtTheSameTime.computeIfAbsent(side, (key) -> new HashSet<>());
                            workedAtTheSameTime.computeIfAbsent(side2, (key) -> new HashSet<>());
                            workedAtTheSameTime.get(side).add(side2);
                            workedAtTheSameTime.get(side2).add(side);
                        }
                    }
                    mutex.release();
                }, (side, layer) -> {
            mutex.acquireUninterruptibly();
            working[side]--;
            mutex.release();
        }, () -> {
        }, () -> {
        });
        rotateCubeRandomly(cube);
        for (int side = 0; side < 6; side++) {
            assertTrue(sets[side].containsAll(workedAtTheSameTime.get(side)));
        }
    }

    void rotateCubeRandomly(Cube cube) {
        rotateCubeRandomly(cube, false);
    }

    void rotateCubeRandomly(Cube cube, boolean useShows) {
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 30; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    for (int rotations = 0; rotations < 1000; rotations++) {
                        cube.rotate((rotations * finalI) % 6, (rotations * finalI) % 5);
                        if (useShows && rotations % 50 == 0) {
                            cube.show();
                        }
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }));
        }
        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    void sameSidesRotateDifferentLayers() {
        int SIZE = 5;
        int[][] working = new int[3][SIZE];
        int[] opposite = new int[6];
        opposite[3] = 1;
        opposite[4] = 2;
        opposite[5] = 0;
        Semaphore mutex = new Semaphore(1);
        AtomicBoolean mistake = new AtomicBoolean(false);
        Cube cube = new Cube(SIZE,
                (side, layer) -> {
                    mutex.acquireUninterruptibly();
                    if (side < 3) {
                        if (++working[side][SIZE - 1 - layer] > 1) {
                            System.out.println("Mistake: more than one thread is trying to rotate layer nr " + (SIZE - 1 - layer) + ", side: " + side);
                            mistake.set(true);
                        }
                    } else {
                        if (++working[opposite[side]][layer] > 1) {
                            System.out.println("Mistake: more than one thread is trying to rotate layer nr " + layer + ", side: " + side);
                            mistake.set(true);
                        }
                    }
                    mutex.release();
                }, (side, layer) -> {
            mutex.acquireUninterruptibly();
            if (side < 3) {
                working[side][SIZE - 1 - layer]--;
            } else {
                working[opposite[side]][layer]--;
            }
            mutex.release();
        }, () -> {
        }, () -> {
        });
        rotateCubeRandomly(cube);
        assertFalse(mistake.get());
    }

    @Test
    void show() {
        AtomicBoolean mistakeHappened = new AtomicBoolean(false);
        int[] working = new int[6];
        Semaphore mutex = new Semaphore(1);
        Cube cube = new Cube(5,
                (side, layer) -> {
                    mutex.acquireUninterruptibly();
                    working[side]++;
                    mutex.release();
                }, (side, layer) -> {
            mutex.acquireUninterruptibly();
            working[side]--;
            mutex.release();
        }, () -> {
            mutex.acquireUninterruptibly();
            for (int side = 0; side < 6; side++) {
                if (working[side] > 0) {
                    System.out.println("Mistake: trying to show cube during rotation of side nr " + side);
                    mistakeHappened.set(true);
                }
            }
            mutex.release();
        }, () -> {
        });
        rotateCubeRandomly(cube, true);
        assertFalse(mistakeHappened.get());
    }

    @Test
    void hasProperColoursCount() {
        int size = 5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        rotateCubeRandomly(cube);
        checkColours(cube, size);
    }

    void checkColours(Cube cube, int size) {
        try {
            int[] count = new int[6];
            String show = cube.show();
            char[] res = show.toCharArray();
            for(char c : res) {
                count[c - '0']++;
            }
            for(char c : res) {
                assertEquals(size * size, count[c - '0'], "side " + c + " has " + (count[c - '0']) + " colours on cube\n" + Arrays.toString(count) + "\n" + Arrays.toString(res));
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Test
    void interrupt() {
        int size = 5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 30; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    for (int rotations = 0; rotations < 1000; rotations++) {
                        cube.rotate((rotations * finalI) % 6, (rotations * finalI) % 5);
                    }
                } catch (InterruptedException e) {
                    //System.out.println("Interrupted but cube has proper state");
                }
            }));
        }
        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            while (!thread.isInterrupted())thread.interrupt();
        }
    }

    @Test
    void checkSideBlock() {
        assertTimeoutPreemptively(Duration.ofMillis(5000), this::notBlockingSameSide);
    }

    void notBlockingSameSide() {
        int size = 5;
        CyclicBarrier barrier = new CyclicBarrier(size);
        Cube cube = new Cube(size,
                (side, layer) -> {
                    try {
                        barrier.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }
                }, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    cube.rotate(0, finalI);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }));
        }
        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    void checkBlockingShow() {
        assertTimeoutPreemptively(Duration.ofMillis(5000), this::notBlockingShow);
    }

    void notBlockingShow() {
        int size = 5;
        CyclicBarrier barrier = new CyclicBarrier(30);
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, () -> {
            try {
                barrier.await();
            } catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }
        }, this::emptyRunnable);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 30; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    cube.show();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }));
        }
        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    void showInterruption() {
        int size = 5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 30; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    for (int rotations = 0; rotations < 1000; rotations++) {
                        cube.show();
                    }
                } catch (InterruptedException e) {
                    //System.out.println("Interrupted but cube has proper state");
                }
            }));
        }
        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.interrupt();
        }
    }

    @Test
    void checkStarving() {
        int size =5;
        Cube cube = new Cube(size, this::emptyBi, this::emptyBi, this::emptyRunnable, this::emptyRunnable);
        List<Thread> threads = new ArrayList<>();
        Thread starving  = new Thread(() -> {
            try {
                for (int rotations = 0; rotations < 1000; rotations++) {
                    cube.rotate((rotations) % 6, (rotations) % 5);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        for (int i = 0; i < 30; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                try {
                    int rotations = 0;
                    while(++rotations > 0) {
                        cube.rotate((rotations * finalI) % 6, (rotations * finalI) % 5);
                    }
                } catch (InterruptedException ignored) {
                }
            }));
        }
        for(Thread thread : threads) {
            thread.start();
        }
        starving.start();
        try{
            starving.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        for(Thread thread : threads) {
            while (!thread.isInterrupted())thread.interrupt();
        }
    }
}