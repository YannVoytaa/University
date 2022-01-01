public class Zywieniowe extends Pole{
    private int ile_daje_jedzenie;
    private int ile_rośnie_jedzenie;
    private int ile_urosło_jedzenie;
    public Zywieniowe(int x, int y, int ile_daje_jedzenie, int ile_rośnie_jedzenie){
        super(x,y);
        this.ile_daje_jedzenie = ile_daje_jedzenie;
        this.ile_rośnie_jedzenie = ile_rośnie_jedzenie;
        this.ile_urosło_jedzenie = ile_rośnie_jedzenie;//na poczatku pozywienie istnieje
    }
    public void symuluj(){
        if(ile_urosło_jedzenie != ile_rośnie_jedzenie) ile_urosło_jedzenie++;
    }
    @Override
    public boolean maPożywienie(){
        return ile_rośnie_jedzenie == ile_urosło_jedzenie;
    }
    @Override
    public void jedzPożywienie(){
        ile_urosło_jedzenie = 0;
    }
    @Override
    public int energiaZPożywienia(){
        return ile_daje_jedzenie;
    }
    @Override
    public String toString() {
        return maPożywienie() ? "X" : "O";
        //pole bez jedzenia - O
        //pole z jedzeniem (w danym momencie symulacji) - X
    }
}
