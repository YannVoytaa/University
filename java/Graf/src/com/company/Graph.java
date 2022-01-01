package com.company;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Queue;

public class Graph {
    private ArrayList<Node> list;
    private int size;

    public Graph(int n) {
        this.size = n;
        this.list = new ArrayList<Node>();
        for (int i = 1; i <= n; i++) {
            list.add(new Node(i));
        }
    }

    public Graph(int n, ArrayList<Node> list) {
        this.size = n;
        this.list = list;
        assert this.size == this.list.size();
    }

    public Node getNode(int id) {
        for (int i = 0; i < size; i++) {
            if (list.get(i).getId() == id) return list.get(i);
        }
        return null;
    }

    public void addNode(int id) {
        this.list.add(new Node(id));
        size++;
    }

    public void addEdge(Node start, Node end) {
        start.addEdge(end);
        end.addEdge(start);
    }

    public void addEdge(Node start, Node end, int weight) {
        start.addEdge(end, weight);
        end.addEdge(start, weight);
    }

    public void addEdge(int start, int end) {
        addEdge(getNode(start), getNode(end));
    }

    public void addEdge(int start, int end, int weight) {
        addEdge(getNode(start), getNode(end), weight);
    }

    public void deleteNode(int id) {
        for (int i = 0; i < size; i++) {
            if (list.get(i).getId() == id) {
                list.remove(i);
                size--;
                return;
            }
        }
    }

    public void deleteEdge(int start, int end) {
        this.getNode(start).deleteEdge(end);
        this.getNode(end).deleteEdge(start);
    }

    private void dijkstra(Node start) {
        Queue<Node> q = new PriorityQueue<>();
        for (int i = 0; i < size; i++) {
            list.get(i).setOdw(false);
            list.get(i).setDist(Integer.MAX_VALUE);
        }
        q.add(start);
        start.setDist(0);
        Node akt;
        ArrayList<Edge> edges;
        int size;
        while (!q.isEmpty()) {
            akt = q.poll();
            if (akt.getOdw()) continue;
            akt.setOdw(true);
            edges = akt.getEdges();
            size = edges.size();
            for (int i = 0; i < size; i++) {
                if (!edges.get(i).getEnd().getOdw() && edges.get(i).getEnd().getDist() > akt.getDist() + edges.get(i).getWeight()) {
                    edges.get(i).getEnd().setDist(akt.getDist() + edges.get(i).getWeight());
                    q.add(edges.get(i).getEnd());
                }
            }
        }
    }

    private void bfs(Node start) {
        Queue<Node> q = new LinkedList<>();
        for (int i = 0; i < size; i++) {
            list.get(i).setOdw(false);
            list.get(i).setDist(0);
        }
        q.add(start);
        start.setOdw(true);
        Node akt;
        ArrayList<Edge> edges;
        int size;
        while (!q.isEmpty()) {
            akt = q.poll();
            edges = akt.getEdges();
            size = edges.size();
            for (int i = 0; i < size; i++) {
                if (!edges.get(i).getEnd().getOdw()) {
                    edges.get(i).getEnd().setOdw(true);
                    edges.get(i).getEnd().setDist(akt.getDist() + 1);
                    q.add(edges.get(i).getEnd());
                }
            }
        }
    }

    public boolean ma_cykl(int start) {
        return ma_cykl(getNode(start));
    }

    public boolean ma_cykl(Node start) {
        for (int i = 0; i < size; i++) {
            list.get(i).setOdw(false);
            list.get(i).setDist(0);
        }
        return cykl(start, null, start);
    }

    private boolean cykl(Node start, Node parent, Node startowy) {
        ArrayList<Edge> edges = start.getEdges();
        start.setOdw(true);
        int size = edges.size();
        boolean res = false;
        for (int i = 0; i < size; i++) {
            if (!edges.get(i).getEnd().getOdw()) res |= cykl(edges.get(i).getEnd(), start, startowy);
            if (edges.get(i).getEnd().getId() == startowy.getId() && parent.getId() != startowy.getId()) res = true;
        }
        return res;
    }

    public int dist(int start, int end) {
        return dist(getNode(start), getNode(end));
    }

    public int dist(Node start, Node end) {
        dijkstra(start);
        return end.getDist();
    }

    private int maxId() {
        int maks = 1;
        for (int i = 0; i < size; i++) {
            if (maks < list.get(i).getId()) maks = list.get(i).getId();
        }
        return maks;
    }

    public Graph mst() {
        FindAndUnion.initFindAndUnion(maxId());
        Queue<Edge> q = new PriorityQueue<Edge>();
        ArrayList<Edge> edges;
        for (int i = 0; i < size; i++) {
            edges = (list.get(i).getEdges());
            int edges_size = edges.size();
            for (int j = 0; j < edges_size; j++) {
                q.add(edges.get(j));
            }
        }
        Graph res = new Graph(0);
        for (int i = 0; i < size; i++) {
            res.addNode(list.get(i).getId());
        }
        Edge akt;
        while (!q.isEmpty()) {
            akt = q.poll();
            //System.out.println(akt.getStart().getId() + " " + akt.getEnd().getId());
            if (FindAndUnion.find(akt.getStart().getId()) != FindAndUnion.find(akt.getEnd().getId())) {
                res.addEdge(akt.getStart().getId(), akt.getEnd().getId(), akt.getWeight());
                FindAndUnion.union(akt.getStart().getId(), akt.getEnd().getId());
            }
        }
        return res;
    }

    @Override
    public String toString() {
        String res = "";
        for (int i = 0; i < size; i++) res = res + list.get(i).toString() + '\n';
        return res;
    }

    private void dfs(Node x) {
        ArrayList<Edge> edges = x.getEdges();
        Node end;
        for (int i = 0; i < edges.size(); i++) {
            end = edges.get(i).getEnd();
            end.deleteEdge(x.getId());
            edges.remove(i);
            i--;
            dfs(end);
        }
        System.out.print(x.getId() + " ");
    }

    public Graph clone() {
        Graph res = new Graph(0);
        for (int i = 0; i < size; i++) {
            res.addNode(list.get(i).getId());
        }
        for (int i = 0; i < size; i++) {
            for (Edge e : list.get(i).getEdges()) {
                res.list.get(i).addEdge(res.getNode(e.getEnd().getId()));
            }
        }
        return res;
    }

    public void euler() {
        for(Node v : list){
            if(v.getEdges().size() %2==1) {
                System.out.println("Nie ma cyklu");
                return;
            }
        }
        Graph clone = clone();
        clone.dfs(clone.list.get(0));
    }
}
