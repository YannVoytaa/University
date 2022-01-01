package com.company;

import java.util.Random;

public class WirusDziwny extends Wirus {
    private static final Random r = new Random();
    protected void mutuj(){
        int dlugosc = kwas.length();
        int poz1 = r.nextInt(dlugosc);
        int poz2 = r.nextInt(dlugosc);
        char temp=kwas.charAt(poz1);
        kwas.setCharAt(poz1, kwas.charAt(poz2));
        kwas.setCharAt(poz2, temp);
    }
    public WirusDziwny(String kwas, int mutacjeWMiesiącu){
        super(kwas, mutacjeWMiesiącu);
    }
}
