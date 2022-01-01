package com.company;

import java.util.Random;

interface IntFunction {
    int run(int str);
}

public abstract class Warzywo {
    protected long czas_zasadzenia;
    protected int koszt;
    protected IntFunction wartosc;
    static Warzywo[] lista_warzyw = {new Pomidor(), new Ziemniak(), new Marchew()};

    public abstract Warzywo clone();

    public abstract String toStringKoszt();

    public static Warzywo randomWarzywo() {
        Random random = new Random();
        return lista_warzyw[random.nextInt(lista_warzyw.length)].clone();
    }

    public int get_wartosc() {
        return this.wartosc.run((int) ((System.currentTimeMillis() - czas_zasadzenia) / 1000));
    }
}
