package com.company;

public abstract class Wirus {
    protected StringBuilder kwas;
    private int mutacjeWMiesiącu;
    protected abstract void mutuj();
    public void symuluj(int miesiące){
        Szczepionka sz = new Szczepionka(kwas.toString());
        for(int i=1;i<=miesiące;i++){
            for(int j=1;j<=mutacjeWMiesiącu;j++){
                mutuj();
            }
        }
        if(sz.czyZadziała(this)) System.out.println("Szczepionka zadziała");
        else System.out.println("Szczepionka nie zadziała");
    }
    public Wirus(String kwas, int mutacjeWMiesiącu){
        this.kwas = new StringBuilder(kwas);
        this.mutacjeWMiesiącu = mutacjeWMiesiącu;
    }
}
