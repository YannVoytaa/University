public class Rob {
    private final StanRoba stan;
    private final double ułamek_energii_rodzica;
    private final double pr_usunięcia_instr;
    private final double pr_dodania_instr;
    private final double pr_zmiany_instr;
    private final double pr_powielenia;
    private final int limit_powielania;
    private int wiek;
    private final Program program;
    private final char[] spis_inst;

    public Rob(Plansza plansza, Pole gdzie_jest, Program pocz_program, int pocz_energia, double pr_powielenia, 
               double ułamek_energii_rodzica, int limit_powielania, double pr_usunięcia_instr, double pr_dodania_instr,
               double pr_zmiany_instr, char[] spis_instr, int dirX, int dirY) {
        stan = new StanRoba(pocz_energia, dirY, dirX, gdzie_jest, plansza);
        this.program = pocz_program;
        this.spis_inst = spis_instr;
        this.pr_powielenia = pr_powielenia;
        this.ułamek_energii_rodzica = ułamek_energii_rodzica;
        this.limit_powielania = limit_powielania;
        this.pr_usunięcia_instr = pr_usunięcia_instr;
        this.pr_dodania_instr = pr_dodania_instr;
        this.pr_zmiany_instr = pr_zmiany_instr;
        this.wiek = 0;
    }
    public Rob(StanRoba stan, Program pocz_program, double pr_powielenia, double ułamek_energii_rodzica, 
               int limit_powielania, double pr_usunięcia_instr, double pr_dodania_instr, 
               double pr_zmiany_instr, char[] spis_instr) {
        this.stan = stan;
        this.program = pocz_program;
        this.spis_inst = spis_instr;
        this.pr_powielenia = pr_powielenia;
        this.ułamek_energii_rodzica = ułamek_energii_rodzica;
        this.limit_powielania = limit_powielania;
        this.pr_usunięcia_instr = pr_usunięcia_instr;
        this.pr_dodania_instr = pr_dodania_instr;
        this.pr_zmiany_instr = pr_zmiany_instr;
        this.wiek = 0;
    }
    
    public Rob potomek() {
        Rob potomek = 
                new Rob(stan.stanZOdwróconymKierunkiem(), 
                        program.mutuj(pr_usunięcia_instr, pr_dodania_instr, pr_zmiany_instr, spis_inst),
                pr_powielenia, ułamek_energii_rodzica, limit_powielania,
                pr_usunięcia_instr, pr_dodania_instr, pr_zmiany_instr, spis_inst);
        stan.setEnergia((int) (stan.energia() - stan.energia() * ułamek_energii_rodzica));
        return potomek;
    }
    public void symuluj(int koszt_tury) {
        for (char c : program.dajInstrukcje()) {
            stan.wykonajRuch(c);
            if (stan.energia() < 0) break;
        }
        stan.setEnergia(stan.energia() - koszt_tury);
        if (stan.energia() < 0) return;
        if (Math.random() < pr_powielenia && limit_powielania <= stan.energia()) {
            stan.plansza().dodajRoba(potomek());
        }
        wiek++;
    }
    @Override
    public String toString() {
        return "Rob{" +
                "Program=" + program.toString() +", " +stan.toString() + "}";
    }
    public int rozmiarProgramu() {
        return program.dajInstrukcje().size();
    }
    public int dajEnergię() {
        return stan.energia();
    }
    public int dajWiek() {
        return wiek;
    }
    public boolean umarł() {
        return stan.energia() < 0;
    }
}
