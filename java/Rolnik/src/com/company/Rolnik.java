package com.company;

import org.junit.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;

interface StrategiaRolnika {
    void run(Garden g, int time, Rolnik r);
}

public class Rolnik {
    private StrategiaRolnika strategia;
    private ArrayList<Warzywo> zebraneWarzywa;
    private int wydatek, przychód;

    public Rolnik(StrategiaRolnika strategia) {
        this.strategia = (a, b, c) -> strategia.run(a, b, c);
        this.zebraneWarzywa = new ArrayList<Warzywo>();
        this.wydatek = 0;
        this.przychód = 0;
    }

    public void dodajDoKoszyka(Warzywo w) {
        zebraneWarzywa.add(w);
    }

    public int daj_wartosc_warzywa(Warzywo w) {
        return w.get_wartosc();
    }

    public void simulate(Garden g, int time) {
        this.strategia.run(g, time, this);
    }

    public void zasadz(Garden g, Warzywo w, int miejsce) {
        this.wydatek += w.koszt;
        System.out.println("Zasadziłem: " + g.zasadz(w, miejsce).toStringKoszt());
    }

    public void zbierz(Garden g, int miejsce) {
        Warzywo zebrane = g.zbierz(miejsce);
        this.przychód += zebrane.get_wartosc();
        System.out.println("Zebrałem: " + zebrane);
        this.dodajDoKoszyka(zebrane);
    }

    public void wypiszZebraneWarzywa() {
        System.out.println("Bilans:");
        for (Warzywo w : zebraneWarzywa) {
            System.out.println(w.toStringKoszt());
        }
        System.out.println("Wydatek: " + this.wydatek);
        System.out.println("Przychód: " + this.przychód);
        System.out.println("Zarobione: " + (this.przychód - this.wydatek));
    }

    public void fillWithRandom(Garden g) {
        for (int i = 0; i < g.getSize(); i++) {
            this.zasadz(g, Warzywo.randomWarzywo(), i);
        }
    }

    public void collectAll(Garden g) {
        for (int i = 0; i < g.getSize(); i++) {
            this.zbierz(g, i);
        }
    }

    public void checkPrices(Garden g, int[] prev) {
        for (int i = 0; i < g.getSize(); i++) {
            if (g.getPlace(i).get_wartosc() < prev[i]) {
                this.zbierz(g, i);
                this.zasadz(g, Warzywo.randomWarzywo(), i);
            }
            prev[i] = g.getPlace(i).get_wartosc();
        }
    }

    @Test
    public void testuj() {
        int wydatekTest=0;
        for (Warzywo w : zebraneWarzywa) {
            wydatekTest += w.koszt;
        }
        assertEquals(wydatekTest, this.wydatek);
    }
}
