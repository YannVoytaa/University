package com.company;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.IntBinaryOperator;

public class MatrixRowSums {

    private static final int ROWS = 10;
    private static final int COLUMNS = 100;

    private static class Matrix {
        private final int[] rowSums;
        private final ConcurrentHashMap<Integer, AtomicInteger> columnsFilled;
        private final BlockingQueue<Integer> rowsDuringProcess;
        private final int rows;
        private final int columns;
        private final IntBinaryOperator definition;
        private final ConcurrentHashMap<Integer, AtomicInteger> rowSumsConcurrent;

        public Matrix(int rows, int columns, IntBinaryOperator definition, int k) {
            this.rows = rows;
            this.columns = columns;
            this.definition = definition;
            this.columnsFilled = new ConcurrentHashMap<>();
            this.rowSums = new int[rows];
            this.rowsDuringProcess = new LinkedBlockingQueue<>(k);
            this.rowSumsConcurrent = new ConcurrentHashMap<>();
        }

        private class Helper implements Runnable {
            private final int column;
            private final boolean printOnly;

            public Helper(int column) {
                this(column, false);
            }

            public Helper(int column, boolean printOnly) {
                this.column = column;
                this.printOnly = printOnly;
            }

            @Override
            public void run() {
                for(int row = 0; row < rows; ++row) {
                    AtomicInteger colsFilled = columnsFilled.computeIfAbsent(row, (key) -> new AtomicInteger(0));
                    AtomicInteger sum = rowSumsConcurrent.computeIfAbsent(row, (key) -> new AtomicInteger(0));
                    if (colsFilled.getAndIncrement() == 0) {
                        try {
                            rowsDuringProcess.put(row);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                    int cellValue = definition.applyAsInt(row, column);
                    sum.addAndGet(cellValue);
                    if (colsFilled.incrementAndGet() == 2 * columns) {
                        if(printOnly) {
                            System.out.println(sum.get());
                        }
                        else {
                            rowSums[row] = sum.get();
                        }
                        rowSumsConcurrent.remove(row);
                        columnsFilled.remove(row);
                        try {
                            int processed = rowsDuringProcess.take();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }

        public int[] rowSums() {
            int[] rowSums = new int[rows];
            for (int row = 0; row < rows; ++row) {
                int sum = 0;
                for (int column = 0; column < columns; ++column) {
                    sum += definition.applyAsInt(row, column);
                }
                rowSums[row] = sum;
            }
            return rowSums;
        }

        public int[] rowSumsConcurrent() {
            Thread[] threads = new Thread[columns];
            for (int column = 0; column < columns; ++column) {
                threads[column] = new Thread(new Helper(column), "Helper" + column);
                threads[column].start();
            }
            for (int column = 0; column < columns; ++column) {
                try {
                    threads[column].join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return rowSums;
        }

        public void rowSumsConcurrentPrintOnly() {
            Thread[] threads = new Thread[columns];
            for (int column = 0; column < columns; ++column) {
                threads[column] = new Thread(new Helper(column, true), "Helper" + column);
                threads[column].start();
            }
            for (int column = 0; column < columns; ++column) {
                try {
                    threads[column].join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    static void testMatrixRunnable(IntBinaryOperator definition) {
        Matrix matrix = new Matrix(ROWS, COLUMNS, definition, 5);

        int[] rowSums = matrix.rowSums();
        int[] rowSumsConcurrent = matrix.rowSumsConcurrent();
        matrix.rowSumsConcurrentPrintOnly();
        System.out.println("Sequential:");
        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(i + " -> " + rowSums[i]);
        }
        System.out.println("Concurrent:");
        for (int i = 0; i < rowSumsConcurrent.length; i++) {
            System.out.println(i + " -> " + rowSumsConcurrent[i]);
        }
        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(rowSums[i] + (rowSums[i] == rowSumsConcurrent[i] ? " = " : " != ") + rowSumsConcurrent[i]);
            if(rowSums[i] != rowSumsConcurrent[i]){
                System.out.println("NOT EQUAL");
                return;
            }
        }
        System.out.println("EQUAL");
    }

    public static void main(String[] args) {
        /*Matrix matrix = new Matrix(ROWS, COLUMNS, (row, column) -> {
            int a = 2 * column + 1;
            return (row + 1) * (a % 4 - 2) * a;
        });

        int[] rowSums = matrix.rowSums();
        int[] rowSumsConcurrent = matrix.rowSumsConcurrent();

        System.out.println("Sequential:");
        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(i + " -> " + rowSums[i]);
        }
        System.out.println("Concurrent:");
        for (int i = 0; i < rowSums.length; i++) {
            System.out.println(i + " -> " + rowSumsConcurrent[i]);
        }*/
        testMatrixRunnable((row, column) -> {
            int a = 2 * column + 1;
            return (row + 1) * (a % 4 - 2) * a;
        });
        testMatrixRunnable((row, column) -> {;
            return row * column;
        });
        testMatrixRunnable((row, column) -> {
            int a = column * column + 1;
            return (row + -12) * (a - 2) * a;
        });
    }

}