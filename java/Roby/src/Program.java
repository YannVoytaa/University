import java.util.ArrayList;
import java.util.Arrays;

public class Program {
    private ArrayList<Character> instrukcje;
    private Program(ArrayList<Character> instrukcje){
        this.instrukcje = instrukcje;
    }
    public Program(char[] instrukcje){
        this.instrukcje = new ArrayList<>();
        for(char c: instrukcje){
            this.instrukcje.add(c);
        }
    }
    public Program(String instrukcje){
        this.instrukcje = new ArrayList<>();
        if(instrukcje == null) return;
        for(char c: instrukcje.toCharArray()){
            this.instrukcje.add(c);
        }
    }
    public ArrayList<Character> dajInstrukcje(){
        return instrukcje;
    }
    public Program mutuj(double pr_usunięcia_instr, double pr_dodania_instr, double pr_zmiany_instr, char[] spis_instr){
        ArrayList<Character> instrukcje_potomka = new ArrayList<>();
        for(char c: instrukcje){
            instrukcje_potomka.add(c);
        }
        if(Math.random() < pr_usunięcia_instr && instrukcje_potomka.size()!=0){
            instrukcje_potomka.remove(instrukcje_potomka.size()-1);
        }
        if(Math.random() < pr_dodania_instr){
            instrukcje_potomka.add(spis_instr[(int) (Math.random()*spis_instr.length)]);
        }
        if(Math.random() < pr_zmiany_instr && instrukcje_potomka.size()!=0){
            instrukcje_potomka.set((int) (Math.random() * instrukcje_potomka.size()), spis_instr[(int) (Math.random()*spis_instr.length)]);
        }
        return new Program(instrukcje_potomka);
    }

    @Override
    public String toString() {
        return "Program{" +
                "instrukcje=" + instrukcje +
                '}';
    }
}
