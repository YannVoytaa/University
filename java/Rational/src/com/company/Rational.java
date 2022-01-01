package com.company;

public class Rational {
    private int numerator, denominator;

    public Rational(int p, int q) {
        int akt_nwd = nwd(abs(p), abs(q));
        this.numerator = p / akt_nwd;
        this.denominator = q / akt_nwd;
        if(this.denominator == 0) throw new ArithmeticException();
        //chcemy, zeby mianownik byl dodatni-
        //jesli jest ujemny to zmieniamy znaki
        if (this.denominator < 0) {
            this.denominator *= -1;
            this.numerator *= -1;
        }
    }

    public Rational(int n) {
        this.numerator = n;
        this.denominator = 1;
    }

    private static int nwd(int a, int b) {
        if (a == 0 || b == 0) return a + b;
        else if (a > b) return nwd(a % b, b);
        else return nwd(a, b % a);
    }

    private static int abs(int x) {
        if (x >= 0) return x;
        else return -x;
    }

    public static Rational Zero() {
        return new Rational(0);
    }

    public static Rational One() {
        return new Rational(1);
    }

    public Rational add(Rational x) {
        return new Rational(this.numerator * x.denominator + x.numerator * this.denominator,
                this.denominator * x.denominator);
    }

    public Rational multiply(Rational x) {
        return new Rational(this.numerator * x.numerator, this.denominator * x.denominator);
    }

    public Rational opposite() {
        return new Rational(-this.numerator, this.denominator);
    }

    public Rational subtract(Rational x) {
        return this.add(x.opposite());
    }

    public Rational inverse() {
        return new Rational(this.denominator, this.numerator);
    }

    public Rational divide(Rational x) {
        return this.multiply(x.inverse());
    }

    public int sign() {
        if (this.numerator > 0) return 1;
        else if (this.numerator < 0) return -1;
        else return 0;
    }

    public int numerator() {
        return this.numerator;
    }

    public int denominator() {
        return this.denominator;
    }

    public int compare(Rational x) {
        //porownujac this z x mozemy sprawdzic znak liczby this-x
        return this.subtract(x).sign();
    }

    public String toString() {
        return this.numerator + "/" + this.denominator;
    }

    @Override
    public boolean equals(Object o){
        if(o==this) return true;
        if(!(o instanceof Rational)) return false;
        Rational x=(Rational) o;
        return this.compare(x)==0;
    }
}
