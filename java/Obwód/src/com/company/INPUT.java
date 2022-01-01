package com.company;
public class INPUT extends Bramka{
    public boolean output() {
        boolean wynik=true;
        for(boolean b: input){
            wynik = b;
        }
        return wynik;
    }
    public INPUT(int size){
        super(size);
    }
}
