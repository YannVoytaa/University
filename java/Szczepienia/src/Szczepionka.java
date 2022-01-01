import java.util.Arrays;

public class Szczepionka {
    private final char[] nazwa;
    private final char[] producent;
    private final int dawka;

    public Szczepionka(char[] nazwa, char[] producent, int dawka) {
        this.dawka = dawka;
        this.nazwa = new char[nazwa.length];
        System.arraycopy(nazwa, 0, this.nazwa, 0, nazwa.length);
        this.producent = new char[producent.length];
        System.arraycopy(producent, 0, this.producent, 0, producent.length);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Szczepionka that = (Szczepionka) o;
        return dawka == that.dawka && Arrays.equals(nazwa, that.nazwa) && Arrays.equals(producent, that.producent);
    }
}
