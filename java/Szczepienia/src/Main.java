public class Main {

    public static void main(String[] args) {
        Wojewodztwo w = new Wojewodztwo(1);
        Wojewodztwo w2 = new Wojewodztwo(2);
        Wojewodztwo w3 = new Wojewodztwo(3);
        Wojewodztwo w4 = new Wojewodztwo(4);
        Wojewodztwo w5 = new Wojewodztwo(5);
        char[] nazwa = {'A'};
        char[] prod = {'B'};
        Szczepionka a = new Szczepionka(nazwa, prod, 20);
        Szczepionka b = new Szczepionka(prod, nazwa, 30);
        PunktSzczepienia p = new PunktSzczepienia(a, 20, 12345);
        PunktSzczepienia p2 = new PunktSzczepienia(b, 20, 12365);
        PunktSzczepienia p3 = new PunktSzczepienia(a, 20, 22365);
        PunktSzczepienia p4 = new PunktSzczepienia(a, 20, 32365);
        Pacjent pac = new Pacjent(12366, a, 5000);
        Pacjent pac2 = new Pacjent(12366, b, 5000);
        //System.out.println(pac.szukajPunktu().dajAdres()); //== 12345
        //System.out.println(pac2.szukajPunktu().dajAdres()); //== 12365
    }
}
