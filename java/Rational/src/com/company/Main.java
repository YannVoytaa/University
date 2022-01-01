package com.company;

public class Main {
    public static boolean check(int x){
        if(x==0)return true;
        if(x%10==0||x%1000==121) return false;
        else return check(x/10);
    }
    public static void main(String[] args) {
        Rational one = Rational.One();
        Rational zero = Rational.Zero();
        assert (one.add(zero).compare(one) == 0);
        assert (one.multiply(zero).compare(one) == -1);
        assert (one.multiply(zero).compare(zero) == 0);
        Rational a = new Rational(7);
        Rational b = new Rational(17, 9);
        Rational c = new Rational(18, 42);
        assert (c.toString().equals("3/7"));
        Rational d = c.add(b);
        assert (d.toString().equals("146/63"));
        Rational e = c.multiply(a);
        assert (e.compare(new Rational(3)) == 0);
        Rational f = new Rational(5, -2);
        assert (f.toString().equals("-5/2"));
        Rational g = f.divide(f.opposite());
        assert (g.opposite().compare(one) == 0);
        assert (f.inverse().multiply(f).compare(one) == 0);
        assert (f.numerator() == -5);
        assert (f.denominator() == 2);
        try{
            Rational h = one.divide(zero);
            //jesli podzieli przez 0 to zachowa sie niepoprawnie
            assert(false);
        } catch (ArithmeticException E) {
            //jesli wyrzuci wyjatek to zachowa sie poprawnie
            assert(true);
        }
        assert(f.equals(f.add(one).subtract(one)));
        assert(!one.equals(zero));
    }
}
