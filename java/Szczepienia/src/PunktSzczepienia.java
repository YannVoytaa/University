public class PunktSzczepienia {
    private final int pacjenci_w_dzien;
    private final int adres;
    Szczepionka czymSzczepi;
    private int pierwszy_wolny_termin;
    private int pacjenci_zapisani_na_pierwszy_wolny_termin;

    public PunktSzczepienia(Szczepionka czymSzczepi, int pacjenci_w_dzien, int adres) {
        this.czymSzczepi = czymSzczepi;
        this.pacjenci_w_dzien = pacjenci_w_dzien;
        this.adres = adres;
        CentralneBiuroSzczepien.dodajPunktSzczepień(this);
    }

    public int dajWolnyTermin() {
        return pierwszy_wolny_termin;
    }

    public void zapiszPacjenta(Pacjent p) {
        pacjenci_zapisani_na_pierwszy_wolny_termin++;
        //jesli caly dzien staje sie zajety
        if (pacjenci_zapisani_na_pierwszy_wolny_termin == pacjenci_w_dzien) {
            pierwszy_wolny_termin++;
            pacjenci_zapisani_na_pierwszy_wolny_termin = 0;
        }
    }

    public void raportujZaszczepienie() {
        CentralneBiuroSzczepien.przyjmijInfoOZaszczepieniu();
    }

    public boolean spełniaWymogi(int adres_pacjenta, Szczepionka s, int odległość) {
        return (s.equals(czymSzczepi) && Math.abs(adres_pacjenta - adres) <= odległość);
    }

    public boolean jestBliżej(PunktSzczepienia drugi_punkt, int adres_pacjenta) {
        return Math.abs(this.adres - adres_pacjenta) < Math.abs(drugi_punkt.adres - adres_pacjenta);
    }

    public int dajAdres() {
        return adres;
    }
}
