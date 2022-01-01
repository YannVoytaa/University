package com.company;

import java.util.Random;

public class WirusSpecyficzny extends Wirus {
    private int[] mutującePozyje;
    private char[] możliweTypyMutacji;
    private static final Random r = new Random();
    protected void mutuj(){
        int poz = mutującePozyje[r.nextInt(mutującePozyje.length)];
        char typ = możliweTypyMutacji[r.nextInt(możliweTypyMutacji.length)];
        kwas.setCharAt(poz, typ);
    }
    public WirusSpecyficzny(String kwas, int mutacjeWMiesiącu, int[] mutującePozyje, char[] możliweTypyMutacji){
        super(kwas, mutacjeWMiesiącu);
        this.mutującePozyje = mutującePozyje;
        this.możliweTypyMutacji = możliweTypyMutacji;
    }
}
