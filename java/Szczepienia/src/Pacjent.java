public class Pacjent {
    private final int adres;//w tym zadaniu adres może być final, bo nie mamy nigdzie zamiaru go zmieniać
    private final Szczepionka czym_chce_się_szczepić;
    private final int max_odległość_do_szczepionki;
    private int kiedy_się_zaszczepi;

    public Pacjent(int adres, Szczepionka czym_chce_się_szczepić, int max_odległość_do_szczepionki) {
        this.adres = adres;
        this.czym_chce_się_szczepić = czym_chce_się_szczepić;//szczepionka ma wszystkie atrybuty stałe, więc nikt jej nie podmieni, więc nie musimy
        //robić głębokiej kopii
        this.max_odległość_do_szczepionki = max_odległość_do_szczepionki;
    }

    public void umów_się(PunktSzczepienia p) {
        kiedy_się_zaszczepi = p.dajWolnyTermin();
        p.zapiszPacjenta(this);
    }

    public PunktSzczepienia szukajPunktu() {
        return CentralneBiuroSzczepien.znajdźPunkt(this.adres, this.czym_chce_się_szczepić, this.max_odległość_do_szczepionki);
    }

    public void szukajPunktuIUmówSię() {
        umów_się(CentralneBiuroSzczepien.znajdźPunkt(this.adres, this.czym_chce_się_szczepić, this.max_odległość_do_szczepionki));
    }
}
