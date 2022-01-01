package com.company;

import java.util.ArrayList;

public class Node implements Comparable<Node> {
    private int id;
    private ArrayList<Edge> edges;
    private boolean odw;
    private int dist;

    public Node(int id) {
        this.id = id;
        this.edges = new ArrayList<>();
    }

    public Node(int id, ArrayList<Edge> list) {
        this.id = id;
        edges = list;
    }

    public int getId() {
        return id;
    }

    public void addEdge(Node end) {
        edges.add(new Edge(this, end));
    }

    public void addEdge(Node end, int weight) {
        edges.add(new Edge(this, end, weight));
    }

    public void deleteEdge(int end) {
        int size = edges.size();
        for (int i = 0; i < size; i++) {
            if (edges.get(i).getEnd().getId() == end) {
                edges.remove(i);
                return;
            }
        }
    }

    public boolean getOdw() {
        return odw;
    }

    public void setOdw(boolean b) {
        odw = b;
    }

    public int getDist() {
        return dist;
    }

    public void setDist(int d) {
        dist = d;
    }

    public ArrayList<Edge> getEdges() {
        return edges;
    }

    @Override
    public String toString() {
        String res = "Node nr " + id + ": ";
        int size = edges.size();
        for (int i = 0; i < size; i++)
            res += "(" + edges.get(i).getEnd().getId() + ", " + edges.get(i).getWeight() + "),";
        return res;
    }

    @Override
    public int compareTo(Node anotherNode) {
        if (this.getDist() < anotherNode.getDist()) return -1;
        else if (this.getDist() == anotherNode.getDist()) return 0;
        else return 1;
    }
}
