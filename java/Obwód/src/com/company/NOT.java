package com.company;
public class NOT extends Bramka{
    public boolean output() {
        boolean wynik=false;
        for(boolean b: input){
            wynik = !b;
        }
        return wynik;
    }
    public NOT(int size){
        super(size);
    }
}
