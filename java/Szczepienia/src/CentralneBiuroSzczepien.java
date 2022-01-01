import java.util.ArrayList;

public class CentralneBiuroSzczepien {
    private static final ArrayList<Wojewodztwo> województwa = new ArrayList<>();
    private static int zaszczepieni;

    private static Wojewodztwo findWojewództwo(int id) {
        for (Wojewodztwo w : województwa) {
            if (w.dajIdentyfikator() == id) {
                return w;
            }
        }
        //zeby nie bylo nulla mozemy sami w tym miejscu stworzyc wojewodztwo o danym id. W rozwiązaniu zakładam, że mimo wszystko województwa istnieją.
        return null;
    }

    public static PunktSzczepienia znajdźPunkt(int adresPacjenta, Szczepionka czymSzczepić, int max_odległość) {
        Wojewodztwo województwo_pacjenta = findWojewództwo(adresPacjenta / 10000);//adres ma 5 cyfr
        assert województwo_pacjenta != null;
        return znajdźPunkt(adresPacjenta, czymSzczepić, max_odległość, województwo_pacjenta.dajPunktySzczepień());
    }

    public static PunktSzczepienia znajdźPunkt(int adresPacjenta, Szczepionka czymSzczepić, int max_odległość, ArrayList<PunktSzczepienia> punktySzczepień) {
        int najszybszy_termin = Integer.MAX_VALUE;
        PunktSzczepienia punkt_z_najszybszym_terminem = null;
        for (PunktSzczepienia p : punktySzczepień) {
            if (p.spełniaWymogi(adresPacjenta, czymSzczepić, max_odległość)) {
                if (najszybszy_termin > p.dajWolnyTermin()) {
                    najszybszy_termin = p.dajWolnyTermin();
                    punkt_z_najszybszym_terminem = p;
                } else if (najszybszy_termin == p.dajWolnyTermin()) {
                    if (p.jestBliżej(punkt_z_najszybszym_terminem, adresPacjenta)) {
                        punkt_z_najszybszym_terminem = p;
                    }
                }
            }
        }
        return punkt_z_najszybszym_terminem;
    }

    public static void przyjmijInfoOZaszczepieniu() {
        zaszczepieni++;
    }

    public static void dodajWojewództwo(Wojewodztwo w) {
        województwa.add(w);
    }

    public static void dodajPunktSzczepień(PunktSzczepienia p) {
        findWojewództwo(p.dajAdres() / 10000).dodajPunktSzczepienia(p);
    }

    public static int ilośćZaszczepionych() {
        return zaszczepieni;
    }
}
