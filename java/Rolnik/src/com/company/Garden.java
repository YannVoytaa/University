package com.company;

public class Garden {
    private Warzywo[] place;
    private int size;

    public Garden(int k) {
        this.place = new Warzywo[k];
        this.size = k;
    }

    public Warzywo zasadz(Warzywo w, int miejsce) {
        if (miejsce >= size || miejsce < 0) {
            return null;
        } else {
            place[miejsce] = w;
            return place[miejsce];
        }
    }

    public Warzywo zbierz(int miejsce) {
        if (miejsce >= size || miejsce < 0) {
            return null;
        } else {
            Warzywo zebrane = place[miejsce];
            return zebrane;
        }
    }

    public int getSize() {
        return this.size;
    }

    public Warzywo getPlace(int i) {
        return place[i];
    }
}
