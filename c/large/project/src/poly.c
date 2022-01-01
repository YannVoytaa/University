/** @file
 Implementacja klasy wielomianów wielu zmiennych.
 @author Jan Wojtach
 @date 2021
 */
#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++) {
            PolyDestroy(&p->arr[i].p);
        }
        free(p->arr);
    }
}

/**
 * Tworzy wielomian z posortowanej rosnąco dynamicznej tablicy jednomianów
 * @param[in] v : tablica jednomianów v,
 * @return wielomian w z tablicy v
 */
static Poly PolyFromVector(struct Vector *v) {
    if (v->size == 0) {
        free(v->arr);
        return PolyFromCoeff(0);
    } else if (v->size == 1 && MonoGetExp(&v->arr[0]) == 0 && PolyIsCoeff(&v->arr[0].p)) {
        Poly wynik = PolyFromCoeff(v->arr[0].p.coeff);
        free(v->arr);
        return wynik;
    } else {
        Poly wynik = {.arr = v->arr, .size = v->size};
        return wynik;
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(p->coeff);
    else {
        Poly x;
        x.size = p->size;
        x.arr = calloc(x.size, sizeof(Mono));
        if (x.arr == NULL) exit(1);
        for (size_t i = 0; i < x.size; i++) {
            x.arr[i] = MonoClone(&p->arr[i]);
        }
        return x;
    }
}

/**
 * Mnoży wielomian przez stałą.
 * @param[in] p : wielomian p
 * @param[in] c : współczynnik c
 * @return p * c
 */
static Poly PolyMulByCoeff(const Poly *p, poly_coeff_t c) {
    Poly zero = PolyZero();
    return PolyAddWithCoeff(&zero, p, c);
}

/**
 * Dodaje dwa wielomiany zakładając, że pierwszy jest współczynnikiem.
 * @param[in] p : wielomian-współczynnik
 * @param[in] q : wielomian
 * @param[in] c : współczynnik, przez który mnożymy wielomian q
 * @return p+q*c
 */
static Poly PolyPlusCoeff(const Poly *p /*coeff*/, const Poly *q, poly_coeff_t c) {
    size_t q_i = 0;
    struct Vector monos;
    Poly newPoly;
    Mono m;
    initVector(&monos);
    while (q_i < q->size) {
        /// jeśli dodajemy współczynnik do wielomianu to albo wielomian ma niezerowy jednomian przy x^0
        if (0 == MonoGetExp(&q->arr[q_i])) {
            newPoly = PolyAddWithCoeff(p, &q->arr[q_i].p, c);
            if (!PolyIsZero(&newPoly)) {
                m = MonoFromPoly(&newPoly, 0);
                addToVector(&monos, &m);
            }
            q_i++;
        }
            /// albo przy x^0 jest 0, wtedy dodajemy współczynnik przed wielomian, aby dalej był posortowany względem wykładników
        else if (q_i == 0) {
            if (!PolyIsZero(p)) {
                m = MonoFromPoly(p, 0);
                addCopyToVector(&monos, &m);
            }
            newPoly = PolyMulByCoeff(&q->arr[q_i].p, c);
            if (!PolyIsZero(&newPoly)) {
                m = MonoFromPoly(&newPoly, MonoGetExp(&q->arr[q_i]));
                addToVector(&monos, &m);
            }
            q_i++;
        } else {
            newPoly = PolyMulByCoeff(&q->arr[q_i].p, c);
            if (!PolyIsZero(&newPoly)) {
                m = MonoFromPoly(&newPoly, MonoGetExp(&q->arr[q_i]));
                addToVector(&monos, &m);
            }
            q_i++;
        }
    }
    return PolyFromVector(&monos);
}

Poly PolyAddWithCoeff(const Poly *p, const Poly *q, poly_coeff_t c) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff + c * q->coeff);
    } else if (PolyIsCoeff(p)) {
        return PolyPlusCoeff(p, q, c);
    }
        /// jeśli q jest współczynnikiem, a p nie, to chcemy stworzyć wielomian p + (q * c) = (q * c) + p * 1
    else if (PolyIsCoeff(q)) {
        Poly newQ = PolyFromCoeff(q->coeff * c);
        return PolyAddWithCoeff(&newQ, p, 1);
    }
    ///ponizej zakladamy, ze wielomiany nie sa wspolczynnikami i, że
    ///exp sa posortowane
    size_t p_i = 0;
    size_t q_i = 0;
    struct Vector monos;
    Poly newPoly;
    Mono m;
    initVector(&monos);
    while (p_i < p->size || q_i < q->size) {
        ///jesli dodalismy juz wszystkie jednomiany z p lub z q
        if (p_i == p->size) {
            newPoly = PolyMulByCoeff(&q->arr[q_i].p, c);
            if (!PolyIsZero(&newPoly)) {
                m = MonoFromPoly(&newPoly, MonoGetExp(&q->arr[q_i]));
                addToVector(&monos, &m);
            }
            q_i++;
        } else if (q_i == q->size) {
            addCopyToVector(&monos, &p->arr[p_i]);
            p_i++;
        }
            ///jesli istnieją jednomiany zarówno z p i z q
        else {
            ///jesli współczynniki są takie same, to je dodajemy do siebie
            if (MonoGetExp(&p->arr[p_i]) == MonoGetExp(&q->arr[q_i])) {
                newPoly = PolyAddWithCoeff(&p->arr[p_i].p, &q->arr[q_i].p, c);
                if (!PolyIsZero(&newPoly)) {
                    m = MonoFromPoly(&newPoly, MonoGetExp(&p->arr[p_i]));
                    addToVector(&monos, &m);
                }
                p_i++;
                q_i++;
            } else if (MonoGetExp(&p->arr[p_i]) < MonoGetExp(&q->arr[q_i])) {
                addCopyToVector(&monos, &p->arr[p_i]);
                p_i++;
            } else {
                newPoly = PolyMulByCoeff(&q->arr[q_i].p, c);
                if (!PolyIsZero(&newPoly)) {
                    m = MonoFromPoly(&newPoly, MonoGetExp(&q->arr[q_i]));
                    addToVector(&monos, &m);
                }
                q_i++;
            }
        }
    }
    return PolyFromVector(&monos);
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    return PolyAddWithCoeff(p, q, 1);
}

/**
 * Porównuje jednomieny względem wykładników
 * @param a : jednomian a
 * @param b : jednomian b
 * @return sgn(a.exp - b.exp)
 */
static int cmp(const void *a, const void *b) {
    Mono mono_a = *(Mono *) a;
    Mono mono_b = *(Mono *) b;
    if (MonoGetExp(&mono_a) < MonoGetExp(&mono_b)) return -1;
    else if (MonoGetExp(&mono_a) == MonoGetExp(&mono_b)) return 0;
    else return 1;
}


Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) return PolyFromCoeff(p->coeff * q->coeff);
    else if (PolyIsCoeff(p)) {
        return PolyMulByCoeff(q, p->coeff);
    } else if (PolyIsCoeff(q)) {
        return PolyMulByCoeff(p, q->coeff);
    } else {
        struct Vector res;
        Poly newPoly;
        Mono m;
        initVector(&res);
        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < q->size; j++) {
                newPoly = PolyMul(&p->arr[i].p, &q->arr[j].p);
                if (!PolyIsZero(&newPoly)) {
                    m = MonoFromPoly(&newPoly, MonoGetExp(&p->arr[i]) + MonoGetExp(&q->arr[j]));
                    addToVector(&res, &m);
                }
            }
        }
        Poly wynik = PolyAddMonos(res.size, res.arr);
        free(res.arr);
        return wynik;
    }
}

Poly PolyNeg(const Poly *p) {
    return PolyMulByCoeff(p, -1);
}

Poly PolySub(const Poly *p, const Poly *q) {
    return PolyAddWithCoeff(p, q, -1);
}

/**
 * Zwraca wiekszy z argumentow
 * @param a : zmienna a
 * @param b : zmienna b
 * @return max(a,b)
 */
static int max(int a, int b) {
    if (a > b)return a;
    else return b;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (var_idx == 0) {
        if (PolyIsCoeff(p)) {
            if (PolyIsZero(p)) return -1;
            else return 0;
        } else {
            int maks = 0;
            for (size_t i = 0; i < p->size; i++) {
                maks = max(maks, MonoGetExp(&p->arr[i]));
            }
            return maks;
        }
    } else if (PolyIsZero(p)) return -1;
    else if (PolyIsCoeff(p)) return 0;
    else {
        int maks = 0;
        for (size_t i = 0; i < p->size; i++) {
            maks = max(maks, PolyDegBy(&p->arr[i].p, var_idx - 1));
        }
        return maks;
    }
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsCoeff(p)) {
        if (PolyIsZero(p)) return -1;
        else return 0;
    } else {
        int maks = 0;
        for (size_t i = 0; i < p->size; i++) {
            maks = max(maks, MonoGetExp(&p->arr[i]) + PolyDeg(&p->arr[i].p));
        }
        return maks;
    }
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return (p->coeff == q->coeff);
    } else if (PolyIsCoeff(p) || PolyIsCoeff(q)) {
        return false;
    } else {
        if (p->size != q->size) return false;
        for (size_t i = 0; i < p->size; i++) {
            if (MonoGetExp(&p->arr[i]) != MonoGetExp(&q->arr[i])) return false;
            if (!PolyIsEq(&p->arr[i].p, &q->arr[i].p)) return false;
        }
        return true;
    }
}

/**
 * Podnosi x do potegi exp
 * @param x : zmienna x
 * @param exp : zmienna exp
 * @return x^exp
 */
static poly_coeff_t my_pow(poly_coeff_t x, poly_exp_t exp) {
    poly_coeff_t res = 1;
    while (exp) {
        if (exp % 2 == 1) res *= x;
        x *= x;
        exp /= 2;
    }
    return res;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p)) return *p;
    else {
        struct Vector monos;
        initVector(&monos);
        Poly power;
        Mono m;
        for (size_t i = 0; i < p->size; i++) {
            power = PolyFromCoeff(my_pow(x, MonoGetExp(&p->arr[i])));
            power = PolyMul(&p->arr[i].p, &power);
            if (PolyIsZero(&power)) {}
            else if (PolyIsCoeff(&power)) {
                m = MonoFromPoly(&power, 0);
                addToVector(&monos, &m);
            } else {
                for (size_t j = 0; j < power.size; j++) addToVector(&monos, &power.arr[j]);
                free(power.arr);
            }
        }
        Poly wynik = PolyAddMonos(monos.size, monos.arr);
        free(monos.arr);
        return wynik;
    }
}

void PrintPoly(Poly *p) {
    if (PolyIsCoeff(p))printf("%ld", p->coeff);
    else {
        for (size_t i = 0; i < p->size; i++) {
            printf("(");
            PrintPoly(&p->arr[i].p);
            printf(",%d)", p->arr[i].exp);
            if (i < p->size - 1)printf("+");
        }
    }
}

/**
 * Tworzy wielomian na podstawie dynamicznej tablicy jednomianów,
 * dostaje całą tablicę z zawartością na własność
 * @param[in] count : rozmiar tablicy
 * @param[in] monos : dynamiczna tablica jednomianów
 * @return wielomian z jednomianów
 */
static Poly PolyAddMonosUni(size_t count, struct Vector *monos) {
    qsort(monos->arr, count, sizeof(struct Mono), cmp);
    Mono m;
    Poly newPoly;
    size_t size = 0;
    for (size_t i = 0; i < count; i++) {
        if (size != 0 && MonoGetExp(&monos->arr[size - 1]) == MonoGetExp(&monos->arr[i])) {
            newPoly = PolyAdd(&monos->arr[size - 1].p, &monos->arr[i].p);
            if (!PolyIsZero(&newPoly)) {
                m = MonoFromPoly(&newPoly, MonoGetExp(&monos->arr[i]));
                MonoDestroy(&monos->arr[size - 1]);
                monos->arr[size - 1] = m;
            } else {
                MonoDestroy(&monos->arr[size - 1]);
                size--;
            }
            MonoDestroy(&monos->arr[i]);
        } else {
            monos->arr[size] = monos->arr[i];
            size++;
        }
    }
    monos->size = size;
    Poly wynik = PolyFromVector(monos);
    return wynik;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0) return PolyFromCoeff(0);
    struct Vector monosZapas;
    initVectorWithCount(&monosZapas, count);
    for (size_t i = 0; i < count; i++) {
        addToVector(&monosZapas, &monos[i]);
    }
    return PolyAddMonosUni(count, &monosZapas);
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (count == 0) return PolyFromCoeff(0);
    struct Vector monosZapas;
    monosZapas.arr = monos;
    monosZapas.size = count;
    monosZapas.capacity = count;
    return PolyAddMonosUni(count, &monosZapas);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0) return PolyFromCoeff(0);
    struct Vector monosZapas;
    initVectorWithCount(&monosZapas, count);
    for (size_t i = 0; i < count; i++) {
        addCopyToVector(&monosZapas, &monos[i]);
    }
    return PolyAddMonosUni(count, &monosZapas);
}

/**
 * Podnosi wielomian do potęgi
 * @param[in] p : wielomian
 * @param[in] exp : wykładnik
 * @return p^exp
 */
static Poly PolyToPower(Poly p, poly_exp_t exp) {
    Poly res = PolyFromCoeff(1);
    Poly base = PolyClone(&p);
    Poly tmp;
    while (exp) {
        if (exp % 2 == 1) {
            tmp = PolyMul(&res, &base);
            PolyDestroy(&res);
            res = tmp;
        }
        tmp = PolyMul(&base, &base);
        PolyDestroy(&base);
        base = tmp;
        exp /= 2;
    }
    PolyDestroy(&base);
    return res;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    if (PolyIsCoeff(p)) {
        return *p;
    } else {
        Poly replace;
        if (k == 0) replace = PolyFromCoeff(0);
        else replace = q[0];
        struct Vector v;
        initVector(&v);
        for (size_t i = 0; i < p->size; i++) {
            Poly internal = PolyCompose(&p->arr[i].p, k == 0 ? k : k - 1, k == 0 ? NULL : q + 1);
            Poly external = PolyToPower(replace, MonoGetExp(&p->arr[i]));
            Poly new = PolyMul(&internal, &external);
            if (PolyIsCoeff(&new)) {
                if (!PolyIsZero(&new)) {
                    Mono m = MonoFromPoly(&new, 0);
                    addToVector(&v, &m);
                }
            } else {
                for (size_t j = 0; j < new.size; j++) {
                    addToVector(&v, &new.arr[j]);
                }
                free(new.arr);
            }
            PolyDestroy(&internal);
            PolyDestroy(&external);
        }
        if (v.size == 0) {
            free(v.arr);
            return PolyZero();
        } else {
            v.capacity = v.size;
            return PolyOwnMonos(v.size, v.arr);
        }
    }
}
