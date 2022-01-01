import java.util.ArrayList;

public class Wojewodztwo {
    private final ArrayList<PunktSzczepienia> punktySzczepień = new ArrayList<>();
    private final int identyfikator; //pierwsza liczba w kodach pocztowych w tym wojewodztwie

    public Wojewodztwo(int identyfikator) {
        this.identyfikator = identyfikator;
        CentralneBiuroSzczepien.dodajWojewództwo(this);
    }

    public Wojewodztwo(int identyfikator, ArrayList<PunktSzczepienia> punktySzczepień) {
        this.identyfikator = identyfikator;
        CentralneBiuroSzczepien.dodajWojewództwo(this);
        for (PunktSzczepienia p : punktySzczepień) {
            dodajPunktSzczepienia(p);
        }
    }

    public int dajIdentyfikator() {
        return identyfikator;
    }

    public void dodajPunktSzczepienia(PunktSzczepienia p) {
        punktySzczepień.add(p);
    }

    public ArrayList<PunktSzczepienia> dajPunktySzczepień() {
        return punktySzczepień;
    }
}
