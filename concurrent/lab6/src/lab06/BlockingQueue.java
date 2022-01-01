package lab06;

import java.util.ArrayDeque;
import java.util.Queue;

public class BlockingQueue<T> {
    private final Queue<T> queue = new ArrayDeque<>();
    private final int capacity;
    
    private boolean producerWaiting = false;
    private boolean consumerWaiting = false;
    private T waitingItem;
    
    public BlockingQueue(int capacity) {
        this.capacity = capacity;
    }

    public synchronized T take() throws InterruptedException {
        while (getSize() == 0 && (!producerWaiting || consumerWaiting) ){
            wait();
        }
        if(producerWaiting) {
            consumerWaiting = true;
            notifyAll();
            while (waitingItem == null) {
                wait();
            }
            T item = waitingItem;
            waitingItem = null;
            consumerWaiting = false;
            return item;
        }
        T item = queue.poll();
        notifyAll();
        return item;
    }

    public synchronized void put(T item) throws InterruptedException {
        while (getSize() == getCapacity() && !producerWaiting && getCapacity() != 0) {
            wait();
        }
        if(getCapacity() == 0) {
            producerWaiting = true;
            notifyAll();
            while (!consumerWaiting) {
                wait();
            }
            waitingItem = item;
            notifyAll();
            producerWaiting = false;
            notifyAll();
            return;
        }
        queue.add(item);
        notifyAll();
    }

    public synchronized int getSize() {
        return queue.size();
    }

    public int getCapacity() {
        return capacity;
    }
}