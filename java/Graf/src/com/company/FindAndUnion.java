package com.company;

public class FindAndUnion {
    private static int[] nodes;

    public static int find(int x) {
        if (nodes[x] == x) return x;
        else {
            nodes[x] = find(nodes[x]);
            return nodes[x];
        }
    }

    public static void union(int first, int second) {
        nodes[find(first)] = find(second);
    }

    public static void initFindAndUnion(int n) {
        nodes = new int[n + 1];
        for (int i = 0; i <= n; i++) nodes[i] = i;
    }
}
