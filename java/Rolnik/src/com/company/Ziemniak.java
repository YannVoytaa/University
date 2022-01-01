package com.company;

public class Ziemniak extends Warzywo {
    public Ziemniak() {
        this.czas_zasadzenia = System.currentTimeMillis();
        this.koszt = 2;
        this.wartosc = time -> {
            if (time < 10) return 0;
            else return 5;
        };
    }

    public Ziemniak clone() {
        return new Ziemniak();
    }

    @Override
    public String toString() {
        return "Ziemniak (" + this.wartosc.run((int) (System.currentTimeMillis() - this.czas_zasadzenia) / 1000) + " PLN).";
    }

    public String toStringKoszt() {
        return "Ziemniak (" + this.koszt + " PLN).";
    }
}
