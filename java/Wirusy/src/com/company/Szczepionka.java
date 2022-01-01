package com.company;

public class Szczepionka {
    private StringBuilder kwasNaKtóryDziała;
    public Szczepionka(String s){
        kwasNaKtóryDziała = new StringBuilder(s);
    }
    public boolean czyZadziała(Wirus w){
        int size=kwasNaKtóryDziała.length()-w.kwas.length()+1;
        for(int i=0;i<size; i++){
            if(kwasNaKtóryDziała.substring(i, i+w.kwas.length()).equals(w.kwas.toString())) return true;
        }
        return false;
    }
}
