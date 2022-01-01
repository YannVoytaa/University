package com.company;

public class Edge implements Comparable<Edge> {
    private Node start, end;
    private int weight;

    public Edge(Node start, Node end, int weight) {
        this.start = start;
        this.end = end;
        this.weight = weight;
    }

    public Edge(Node start, Node end) {
        this.start = start;
        this.end = end;
        this.weight = 1;
    }

    public Node getEnd() {
        return end;
    }

    public Node getStart() {
        return start;
    }

    public int getWeight() {
        return weight;
    }

    @Override
    public int compareTo(Edge anotherEdge) {
        if (this.weight < anotherEdge.weight) return -1;
        else if (this.weight == anotherEdge.weight) return 0;
        else return 1;
    }
}
