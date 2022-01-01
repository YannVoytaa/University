package com.company;

import org.junit.Test;

public class Main {
    public static StrategiaRolnika strat = (g, time, r) -> {
        while (time >= 10) {
            r.fillWithRandom(g);
            try {
                Thread.sleep(10000);
            } catch (Exception e) {
                System.out.println("?");
            }
            r.collectAll(g);
            time -= 10;
        }
    };
    public static StrategiaRolnika strat2 = (g, time, r) -> {
        r.fillWithRandom(g);
        int[] prev = new int[g.getSize()];
        while (time >= 0) {
            r.checkPrices(g, prev);
            try {
                Thread.sleep(1000);
            } catch (Exception e) {
                System.out.println("?");
            }
            time--;
        }
        r.collectAll(g);
    };

    public static void main(String[] args) {
        Garden g = new Garden(7);
        Rolnik farmer = new Rolnik(strat);
        System.out.println("Farmer 1:");
        farmer.simulate(g, 25);
        farmer.wypiszZebraneWarzywa();
        farmer.testuj();
        System.out.println("--------------------------");
        System.out.println("Farmer 2:");
        Rolnik farmer2 = new Rolnik(strat2);
        farmer2.simulate(g, 25);
        farmer2.wypiszZebraneWarzywa();
        System.out.println("--------------------------");
        farmer2.testuj();
    }
}
