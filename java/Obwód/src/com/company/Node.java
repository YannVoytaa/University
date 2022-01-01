package com.company;
import jdk.swing.interop.SwingInterOpUtils;

import java.util.ArrayList;
import java.util.Scanner;

public class Node {
    private static Scanner scanner =new Scanner(System.in);
    private int id;
    protected Bramka b;
    private ArrayList<Node> next;
    public boolean isFull(){
        return b.isFull();
    }
    public void push(boolean d){
        b.push(d);
    }
    public int getSize(){
        return b.getSize();
    }
    public void setSize(int size){
        b.setSize(size);
    }
    public void execute(){
        if(next!=null){
            //System.out.println(id+" "+b.output());
            for(Node v:next){
                v.b.push(b.output());
                if(v.isFull()) v.execute();
            }
        }
    }
    public void clear(){
        b.input.clear();
    }
    public int depth(){
        int maks=0;
        for(Node v:next){
            maks=Math.max(maks, 1+v.depth());
        }
        return maks;
    }
    public int getId(){
        return id;
    }
    public Node(int id, String type){
        this.id=id;
        this.next = new ArrayList<>();
        switch(type){
            case "AND": b=new AND(1); break;
            case "OR": b=new OR(1); break;
            case "MAJORITY": b=new MAJORITY(1); break;
            case "NOT": b=new NOT(1); break;
            case "PARITY": b=new PARITY(1); break;
            case "INPUT": b=new INPUT(1); break;
        }
    }
    public Node(int id, String type, int size){
        this.id=id;
        this.next = new ArrayList<>();
        switch(type){
            case "AND": b=new AND(size); break;
            case "OR": b=new OR(size); break;
            case "MAJORITY": b=new MAJORITY(size); break;
            case "NOT": b=new NOT(size); break;
            case "PARITY": b=new PARITY(size); break;
            case "INPUT": b=new INPUT(size); break;
        }
    }
    public void setNext(Node v){
        next.add(v);
    }
    public boolean result(){
        return this.b.output();
    }
}
/*
5
AND
4
OR
3
PARITY
2
NOT
1
MAJORITY
2
7
1 6
OR
2 6
3 7
AND
4 7
5 7
6 8
PARITY
7 8
 */