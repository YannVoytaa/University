public class StanRoba {
    private int dirY;
    private int dirX;
    private Pole gdzie_jest;
    private Plansza plansza;
    private int energia;

    public StanRoba(int energia, int dirY, int dirX, Pole gdzie_jest, Plansza plansza) {
        this.energia = energia;
        this.plansza = plansza;
        this.dirX = dirX;
        this.dirY = dirY;
        this.gdzie_jest = gdzie_jest;
    }
    
    public StanRoba stanZOdwróconymKierunkiem(){
        return new StanRoba(energia, -dirY, -dirX, gdzie_jest, plansza);
    }

    public Plansza plansza() {
        return plansza;
    }

    public int energia() {
        return energia;
    }

    public void setEnergia(int x) {
        energia = x;
    }

    private void skręć(int kierunek) {
        int temp = dirX;
        dirX = kierunek * dirY;
        dirY = kierunek * temp * -1;
    }

    private void idź() {
        gdzie_jest = plansza.sąsiad(gdzie_jest, dirX, dirY);
        if (gdzie_jest.maPożywienie()) {
            gdzie_jest.jedzPożywienie();
            energia += gdzie_jest.energiaZPożywienia();
        }
    }

    private void wąchaj() {
        if (plansza.sąsiad(gdzie_jest, -1, 0).maPożywienie()) {
            dirX = -1;
            dirY = 0;
        } else if (plansza.sąsiad(gdzie_jest, 1, 0).maPożywienie()) {
            dirX = 1;
            dirY = 0;
        } else if (plansza.sąsiad(gdzie_jest, 0, -1).maPożywienie()) {
            dirX = 0;
            dirY = -1;
        } else if (plansza.sąsiad(gdzie_jest, 0, 1).maPożywienie()) {
            dirX = 0;
            dirY = 1;
        }
    }

    private void jedz() {
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0) continue;
                if (plansza.sąsiad(gdzie_jest, x, y).maPożywienie()) {
                    gdzie_jest = plansza.sąsiad(gdzie_jest, x, y);
                    gdzie_jest.jedzPożywienie();
                    energia += gdzie_jest.energiaZPożywienia();
                    return;
                }
            }
        }
    }

    public void wykonajRuch(char c) {
        switch (c) {
            case 'l': skręć(-1); break;
            case 'p': skręć(1); break;
            case 'i': idź(); break;
            case 'w': wąchaj(); break;
            case 'j': jedz(); break;
        }
        energia--;
    }

    public String kierunek() {
        if (dirX == 0) {
            if (dirY == -1) return "Lewo";
            else return "Prawo";
        } else if (dirX == -1) {
            return "Góra";
        } else {
            return "Dół";
        }
    }

    @Override
    public String toString() {
        return  "pole= (" + gdzie_jest.getX() + ", " + gdzie_jest.getY() + ")" +
                ", energia=" + energia +
                ", kierunek=" + kierunek();
    }
}
