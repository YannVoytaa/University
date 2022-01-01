package com.company;
public class OR extends Bramka{
    public boolean output() {
        boolean wynik=false;
        for(boolean b: input){
            wynik |=b;
        }
        return wynik;
    }
    public OR(int size){
        super(size);
    }
}
