import java.util.ArrayList;

public class Plansza {
    private int rozmiar_planszy_x;
    private int rozmiar_planszy_y;
    private Pole[][] pola;
    private ArrayList<Rob> roby = new ArrayList<>();
    private ArrayList<Rob> zapasowa = new ArrayList<>();
    public Plansza(int rozmiar_planszy_x, int rozmiar_planszy_y, ArrayList<char[]> plansza,
                   int pocz_ile_robów, Program pocz_progr, int pocz_energia, int ile_daje_jedzenie,
                   int ile_rośnie_jedzenie, double pr_powielenia, double ułamek_energii_rodzica, int limit_powielania,
                   double pr_usunięcia_instr, double pr_dodania_instr, double pr_zmiany_instr, char[] spis_instr) {
        this.rozmiar_planszy_x = rozmiar_planszy_x;
        this.rozmiar_planszy_y = rozmiar_planszy_y;
        this.pola = new Pole[rozmiar_planszy_x][rozmiar_planszy_y];
        for(int i=0;i<rozmiar_planszy_x;i++){
            for(int j=0;j<rozmiar_planszy_y;j++){
                if(plansza.get(i)[j] == 'x') pola[i][j] = new Zywieniowe(i, j, ile_daje_jedzenie, ile_rośnie_jedzenie);
                else pola[i][j]=new Pole(i, j);
            }
        }
        for(int i=1;i<=pocz_ile_robów;i++){
            int x= (int)(Math.random()*rozmiar_planszy_x);
            int y= (int)(Math.random()*rozmiar_planszy_y);
            int side = (int) (Math.random() * 2);
            int mnożnik = ((int) (Math.random() * 2)) * 2 -1;
            roby.add(new Rob(this, pola[x][y], pocz_progr, pocz_energia, pr_powielenia, ułamek_energii_rodzica,
                    limit_powielania, pr_usunięcia_instr, pr_dodania_instr, pr_zmiany_instr, spis_instr, 
                    side * mnożnik, (1-side) * mnożnik));

        }
    }
    private void wypiszStanSymulacji(){
        for(Rob r: roby){
            System.out.println(r);
        }
        for(Pole[] rząd : pola){
            for(Pole p : rząd){
                System.out.print(p);
            }
            System.out.println();
        }
    }
    private void wypiszPodstawoweDane(int tura, int liczbaRobów, int PolaZŻywnością, int minprg, double śrprg, int maksprg,
                                      int minenerg, double śrenerg, int maksenerg, int minwiek, double śrwiek, int makswiek){
        System.out.print(tura+", ");
        System.out.print("rob: "+liczbaRobów+", ");
        System.out.print("żyw: "+PolaZŻywnością+", ");
        System.out.print("prg: " + minprg+"/");
        System.out.print(śrprg+"/");
        System.out.print(maksprg+", ");
        System.out.print("energ: "+minenerg+"/");
        System.out.print(śrenerg+"/");
        System.out.print(maksenerg+", ");
        System.out.print("wiek: "+minwiek+"/");
        System.out.print(śrwiek+"/");
        System.out.println(makswiek);
    }
    private int min(int a, int b){
        return a < b ? a : b;
    }
    private int max(int a, int b){
        return a > b ? a : b;
    }
    public void symuluj(int ile_tur, int koszt_tury, int co_ile_wypisz){
        wypiszStanSymulacji();
        for(int i=1;i<=ile_tur;i++){
            int liczbaRobów=0, polaZŻywnością=0, minprg=Integer.MAX_VALUE, maksprg=0, minenerg=Integer.MAX_VALUE, 
                    maksenerg=0, minwiek=Integer.MAX_VALUE, makswiek=0;
            int sumaprg=0, sumaenerg=0, sumawiek=0;
            for(Pole[] rząd : pola){
                for(Pole p : rząd){
                    p.symuluj();
                }
            }
            for(Rob r:roby) {
                r.symuluj(koszt_tury);
            }
            for(Pole[] rząd : pola){
                for(Pole p : rząd){
                    if(p.maPożywienie()) polaZŻywnością++;
                }
            }
            for(Rob r:zapasowa) {
                roby.add(r);
            }
            zapasowa.clear();
            for(Rob r:roby){
                if(r.umarł())continue;
                liczbaRobów++;
                minprg = min(minprg, r.rozmiarProgramu());
                maksprg = max(maksprg, r.rozmiarProgramu());
                sumaprg += r.rozmiarProgramu();
                minenerg = min(minenerg, r.dajEnergię());
                maksenerg = max(maksenerg, r.dajEnergię());
                sumaenerg += r.dajEnergię();
                minwiek = min(minwiek, r.dajWiek());
                makswiek = max(makswiek, r.dajWiek());
                sumawiek += r.dajWiek();
                zapasowa.add(r);
            }
            roby.clear();
            for(Rob r:zapasowa){
                roby.add(r);
            }
            zapasowa.clear();
            wypiszPodstawoweDane(i, liczbaRobów, polaZŻywnością, minprg, (double) sumaprg / (double) liczbaRobów, 
                    maksprg, minenerg, (double) sumaenerg / (double) liczbaRobów, maksenerg, minwiek, 
                    (double) sumawiek / (double) liczbaRobów, makswiek);
            if(i % co_ile_wypisz == 0) wypiszStanSymulacji();
        }
        wypiszStanSymulacji();
    }
    public void dodajRoba(Rob r){
        zapasowa.add(r);
    }
    public Pole sąsiad(Pole p, int dirX, int dirY){
        return pola[((p.getX()+dirX)+rozmiar_planszy_x)%rozmiar_planszy_x][((p.getY()+dirY)+rozmiar_planszy_y)%rozmiar_planszy_y];
    }
}
