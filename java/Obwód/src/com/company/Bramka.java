package com.company;
import java.util.ArrayList;

public abstract class Bramka {
    protected ArrayList<Boolean> input;
    private int size;
    public boolean isFull(){
        return (size == input.size());
    }
    public abstract boolean output();
    public void push(boolean b){
        input.add(b);
    }
    public int getSize(){
        return size;
    }
    public void setSize(int size){
        this.size = size;
    }
    private ArrayList<Boolean> getInput(){
        return input;
    }
    public Bramka(int size){
        input=new ArrayList<>();
        this.size=size;
    }
}
