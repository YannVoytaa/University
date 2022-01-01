package com.company;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        test();
        Graph g = new Graph(10);
        Scanner scanner = new Scanner(System.in);
        int krawedzie = scanner.nextInt();
        int a, b, c;
        while (krawedzie-- != 0) {
            a = scanner.nextInt();
            b = scanner.nextInt();
            c = scanner.nextInt();
            g.addEdge(a, b, c);
        }
        g.euler();
        System.out.println("Dystans:");
        int zapy = scanner.nextInt();
        while (zapy-- != 0) {
            a = scanner.nextInt();
            b = scanner.nextInt();
            System.out.println(g.dist(a, b));
        }
        System.out.println("Cykl");
        zapy = scanner.nextInt();
        while (zapy-- != 0) {
            a = scanner.nextInt();
            System.out.println(g.ma_cykl(a));
        }
        System.out.println(g.mst());
    }

    @Test
    public static void test() {
        Graph g = new Graph(10);
        g.addEdge(1, 2, 3);
        g.addEdge(1, 3, 4);
        g.addEdge(2, 4, 5);
        g.addEdge(3, 4, 2);
        g.addEdge(3, 6, 6);
        g.addEdge(4, 5, 4);
        g.addEdge(5, 6, 6);
        g.addEdge(6, 7, 3);
        g.addEdge(7, 8, 3);
        assertEquals(g.dist(1, 4), 6);
        assertEquals(g.dist(7, 4), 11);
        assertEquals(g.dist(1, 9), Integer.MAX_VALUE);
        assertEquals(g.dist(2, 3), 7);
        assertEquals(g.dist(6, 4), 8);
        assertEquals(g.dist(9, 10), Integer.MAX_VALUE);
        assertEquals(g.ma_cykl(1), true);
        assertEquals(g.ma_cykl(6), true);
        assertEquals(g.ma_cykl(7), false);
        assertEquals(g.ma_cykl(9), false);
        assertEquals(g.mst().dist(2, 4), 9);
        assertEquals(g.dist(2, 4), 5);
    }
}
/*
9
1 2 3
1 3 4
2 4 5
3 4 2
3 6 6
4 5 4
5 6 6
6 7 3
7 8 3
 */
/*
11
1 2 1
2 3 1
3 4 1
4 5 1
5 6 1
6 7 1
7 1 1
6 9 1
9 7 1
6 8 1
7 8 1
 */