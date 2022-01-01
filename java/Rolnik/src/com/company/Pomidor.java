package com.company;

public class Pomidor extends Warzywo {
    public Pomidor() {
        this.czas_zasadzenia = System.currentTimeMillis();
        this.koszt = 3;
        this.wartosc = time -> {
            if (time <= 10) return 0;
            else if (time <= 15) return (time - 10) * 2;
            else if (time <= 20) return 10 - (time - 15) * 2;
            else return 0;
        };
    }

    public Pomidor clone() {
        return new Pomidor();
    }

    @Override
    public String toString() {
        return "Pomidor (" + this.wartosc.run((int) (System.currentTimeMillis() - this.czas_zasadzenia) / 1000) + " PLN).";
    }

    public String toStringKoszt() {
        return "Pomidor (" + this.koszt + " PLN).";
    }
}
