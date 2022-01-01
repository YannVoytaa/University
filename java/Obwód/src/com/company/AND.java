package com.company;
public class AND extends Bramka{
    public boolean output() {
        boolean wynik=true;
        for(boolean b: input){
            wynik &= b;
        }
        return wynik;
    }
    public AND(int size){
        super(size);
    }
}