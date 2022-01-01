package com.company;
public class PARITY extends Bramka{
    public boolean output() {
        int wynik=0;
        for(boolean b: input){
            wynik+= b? 1:0;
        }
        return wynik % 2 == 1;
    }
    public PARITY(int size){
        super(size);
    }
}
