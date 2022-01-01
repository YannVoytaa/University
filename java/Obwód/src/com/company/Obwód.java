package com.company;
import java.util.ArrayList;
import java.util.Scanner;

public class Obwód {
    private static Scanner scanner =new Scanner(System.in);
    private ArrayList<Node> inputNodes;
    private ArrayList<Node> normalNodes;
    private Node outputNode;
    public int inputLength(){
        int wynik=0;
        for(Node v:inputNodes){
            wynik+=v.getSize();
        }
        return wynik;
    }
    public void clear(){
        for(Node v:inputNodes){
            v.clear();
        }
        for(Node v:normalNodes){
            v.clear();
        }
    }
    public boolean evaluate(boolean[] input){
        assert input.length == inputLength();
        int ind=0;
        for(Node v:inputNodes){
            while(!v.isFull()){
                v.push(input[ind++]);
            }
            v.execute();
        }
        boolean wynik= outputNode.result();
        return wynik;
    }
    public int depth(){
        int maks=0;
        for(Node v:inputNodes){
            maks =Math.max(maks, v.depth());
        }
        return maks;
    }
    public int size(){
        return inputNodes.size()+normalNodes.size();
    }
    public Obwód(int n){
        inputNodes = new ArrayList<>();
        normalNodes = new ArrayList<>();
        for(int i=1;i<=n;i++){
            inputNodes.add(new Node(i, "INPUT"));
            //System.out.println("Rozmiar inputu dla id="+i+": ");
            //inputNodes.get(i-1).setSize(scanner.nextInt());
        }
    }
    public Node getNormalNode(int id){
        for(Node v:normalNodes){
            if(v.getId() == id) return v;
        }
        return null;
    }
    public Node getNode(int id){
        if(id<=0) return null;
        if(id<=inputNodes.size()) return inputNodes.get(id-1);
        else return getNormalNode(id);
    }
    public void createNode(int id, String s){
        normalNodes.add(new Node(id, s));
    }
    public void createNode(int id, String s, int size){
        normalNodes.add(new Node(id, s, size));
    }
    public void makeNodeOutput(int id){
        Node v=getNode(id);
        outputNode=v;
    }
    public void addEdge(int start, int end){
        if(getNode(start)==null) assert false;
        Node ending = getNode(end);
        if(ending==null) assert false;
        getNode(start).setNext(ending);
        //ending.setSize(ending.getSize()+1);
    }
}
