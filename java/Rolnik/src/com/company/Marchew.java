package com.company;

public class Marchew extends Warzywo {
    public Marchew() {
        this.czas_zasadzenia = System.currentTimeMillis();
        this.koszt = 4;
        this.wartosc = time -> {
            if (time <= 10) return 0;
            else if (time <= 15) return (time - 10) * 2;
            else if (time <= 25) return 10 - (time - 15);
            else return 0;
        };
    }

    public Marchew clone() {
        return new Marchew();
    }

    @Override
    public String toString() {
        return "Marchew (" + this.wartosc.run((int) (System.currentTimeMillis() - this.czas_zasadzenia) / 1000) + " PLN).";
    }

    public String toStringKoszt() {
        return "Marchew (" + this.koszt + " PLN).";
    }
}
