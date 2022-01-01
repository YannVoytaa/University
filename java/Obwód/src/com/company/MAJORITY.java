package com.company;
public class MAJORITY extends Bramka{
    public boolean output() {
        int wynik=0;
        for(boolean b: input){
            wynik+= b? 1:-1;
        }
        return wynik>0;
    }
    public MAJORITY(int size){
        super(size);
    }
}
