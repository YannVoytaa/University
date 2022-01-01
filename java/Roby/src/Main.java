import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        Read reader = Read.createRead();
        reader.readParams();
        int ile_tur = reader.getInt("ile_tur");
        ArrayList<char[]> plansza = reader.getPlansza();
        int rozmiar_planszy_x = reader.getInt("rozmiar_planszy_x");
        int rozmiar_planszy_y = reader.getInt("rozmiar_planszy_y");
        int pocz_ile_robów = reader.getInt("pocz_ile_robów");
        Program pocz_progr = new Program(reader.get("pocz_progr"));
        int pocz_energia = reader.getInt("pocz_energia");
        int ile_daje_jedzenie = reader.getInt("ile_daje_jedzenie");
        int ile_rośnie_jedzenie = reader.getInt("ile_rośnie_jedzenie");
        int koszt_tury = reader.getInt("koszt_tury");
        double pr_powielenia = reader.getDouble("pr_powielenia");
        double ułamek_energii_rodzica = reader.getDouble("ułamek_energii_rodzica");
        int limit_powielania = reader.getInt("limit_powielania");
        double pr_usunięcia_instr = reader.getDouble("pr_usunięcia_instr");
        double pr_dodania_instr = reader.getDouble("pr_dodania_instr");
        double pr_zmiany_instr = reader.getDouble("pr_zmiany_instr");
        String spis_instr = reader.get("spis_instr");
        int co_ile_wypisz = reader.getInt("co_ile_wypisz");
        for(char c:spis_instr.toCharArray()){
            if(c!='l'&&c!='p'&&c!='i'&&c!='j'&&c!='w'){
                System.out.println("Zły spis instrukcji");
                System.exit(1);
            }
        }
        for(char c:pocz_progr.dajInstrukcje()){
            if(c!='l'&&c!='p'&&c!='i'&&c!='j'&&c!='w'){
                System.out.println("Zły początkowy prgoram");
                System.exit(1);
            }
        }
        Plansza p = new Plansza(rozmiar_planszy_x,
                rozmiar_planszy_y,
                plansza,
                pocz_ile_robów,
                pocz_progr,
                pocz_energia,
                ile_daje_jedzenie,
                ile_rośnie_jedzenie,
                pr_powielenia,
                ułamek_energii_rodzica,
                limit_powielania,
                pr_usunięcia_instr,
                pr_dodania_instr,
                pr_zmiany_instr,
                spis_instr.toCharArray());
        p.symuluj(ile_tur, koszt_tury, co_ile_wypisz);
    }
}
