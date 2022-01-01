/** @file
 Implementacja kalkulatora wielamianów wielu zmiennych.
 @author Jan Wojtach
 @date 2021
 */
#include <string.h>
#include "poly.h"
#include "Stack.h"
#include "Vector.h"
#include <errno.h>
#include <stdio.h>
#include <expat.h>
#include "CalcFunctions.h"
/**
 * maksymalna wartość wykładnika
 */
#define EXP_MAX 2147483647
/**
 * najmniejszy kod ascii białego znaku
 */
#define LOWER_WHITE_LIMIT (char)9
/**
 * największy kod ascii białego znaku
 */
#define UPPER_WHITE_LIMIT (char)13
/**
 * kod ascii spacji
 */
#define SPACE 32
/**
 * Znak Hashtag '#'
 */
#define HASH '#'

int isWhite(const char *c) {
    return (*c == SPACE || (*c >= LOWER_WHITE_LIMIT && *c <= UPPER_WHITE_LIMIT));
}

/**
 * Sprawdza, czy znak jest cyfrą.
 * @param[in] c : znak
 * @return c jest cyfrą
 */
static int is_digit(const char *c) {
    return (*c >= '0' && *c <= '9');
}

int startsWithLetter(char *s) {
    return (*s >= 'a' && *s <= 'z') ||
           (*s >= 'A' && *s <= 'Z');
}

int isComment(const char *s) {
    return (*s == HASH);
}

int isEmpty(char *s, int size) {
    return size == 0 || (size == 1 && *s == '\n');
}

int hasNull(char *s, size_t ind) {
    for (size_t i = 0; i < ind; i++)if (s[i] == 0) return 1;
    return 0;
}

int startsWith(char *s1, char *s2) {
    int contains_one_word = 1;
    if (strcmp(s2, "DEG_BY ") == 0 || strcmp(s2, "AT ") == 0 || strcmp(s2, "DEG_BY") == 0 ||
        strcmp(s2, "AT") == 0 || strcmp(s2, "COMPOSE ") == 0 || strcmp(s2, "COMPOSE") == 0)
        contains_one_word = 0;
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    if (*s1 == 0 && *s2 != 0) return 0;//s1 skonczyl sie szybciej
    else if (*s1 != -1 && *s1 != '\n' && contains_one_word == 1 && *s1 != 0)
        return 0; ///istnieje cos niechcianego po komendzie
    else return 1;
}

/**
 * Wypisuje pewną własność wielomianu z góry stosu.
 * @param[in] p : stos
 * @param[in] fun : funkcja mówiąca, czy dana własność jest prawdziwa
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 */
static void topAndPrint(struct Stack *p, bool (*fun)(const Poly *), int *error) {
    if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
    else {
        Poly new = top(p);
        printf(fun(&new) ? "1\n" : "0\n");
    }
}

/**
 * Wrzuca na stos wielomian stworzony przez funkcję.
 * @param[in] p : stos
 * @param[in] fun : funkcja tworząca wielomian
 */
static void getAndPush(struct Stack *p, Poly (*fun)()) {
    Poly new = fun();
    push(p, &new);
}

/**
 * Wrzuca na stos wielomian stworzony przez funkcję jednoargumentową
 * @param[in] p : stos
 * @param[in] fun : funkcja tworząca wielomian
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 * @param[in] shouldPop : czy wielomian z góry powinien zostać usunięty
 */
static void oneArg(struct Stack *p, Poly (*fun)(const Poly *), int *error, int shouldPop) {
    if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
    else {
        Poly top1;
        if (shouldPop == 1) top1 = pop(p);
        else top1 = top(p);
        Poly new = fun(&top1);
        push(p, &new);
        if (shouldPop == 1) PolyDestroy(&top1);
    }
}

/**
 * Wrzuca na stos wielomian stworzony przez funkcję dwuargumentową
 * @param[in] p : stos
 * @param[in] fun : funkcja tworząca wielomian
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 */
static void twoArgs(struct Stack *p, Poly (*fun)(const Poly *, const Poly *), int *error) {
    if (p->size <= 1) *error = ERROR_STACK_UNDERFLOW;
    else {
        Poly top1 = pop(p);
        Poly top2 = pop(p);
        Poly new = fun(&top1, &top2);
        push(p, &new);
        PolyDestroy(&top1);
        PolyDestroy(&top2);
    }
}

/**
 * Wykonuje operację na podstawie linii.
 * @param[in] s : linia
 * @param[in] p : Stos, na którym potencjalnie wykona się operacja
 * @param[in] error : wskaźnik na miejsce, w którym przechowywane są informacje o błędach
 */
static void scanIrregularOperation(char *s, struct Stack *p, int *error) {
    Poly new;
    if (startsWith(s, "DEG_BY ")) {
        char *endptr;
        errno = 0;
        unsigned long idx = strtoul(s + 7, &endptr, 10);
        if (*endptr != -1 && *endptr != '\n' && *endptr != 0) {
            *error = ERROR_DEG_BY;
            return;
        } else if (errno != 0 || *(s + 7) == '-' || *(s + 7) == '+' || isWhite(s + 7)) {
            *error = ERROR_DEG_BY;
            return;
        }
        if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
        else {
            new = top(p);
            printf("%d\n", PolyDegBy(&new, idx));
        }
    } else if (startsWith(s, "DEG_BY")) {
        if (isWhite(s + 6))*error = ERROR_DEG_BY;
        else *error = 1;
    } else if (startsWith(s, "DEG")) {
        if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
        else {
            new = top(p);
            printf("%d\n", PolyDeg(&new));
        }
    } else if (startsWith(s, "POP")) {
        if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
        else {
            new = pop(p);
            PolyDestroy(&new);
        }
    } else if (startsWith(s, "COMPOSE ")) {
        char *endptr;
        errno = 0;
        unsigned long k = strtoul(s + 8, &endptr, 10);
        if (*endptr != -1 && *endptr != '\n' && *endptr != 0) {
            *error = ERROR_COMPOSE;
            return;
        } else if (errno != 0 || *(s + 8) == '-' || *(s + 8) == '+' || isWhite(s + 8)) {
            *error = ERROR_COMPOSE;
            return;
        }
        if (p->size <= k) *error = ERROR_STACK_UNDERFLOW;
        else if (k == 0) {
            new = pop(p);
            Poly composed = PolyCompose(&new, k, NULL);
            push(p, &composed);
            PolyDestroy(&new);
        } else {
            new = pop(p);
            Poly *tab = malloc(k * sizeof(Poly));
            if (tab == NULL) exit(1);
            for (unsigned long i = k - 1;; i--) {
                tab[i] = pop(p);
                if (i == 0)break;
            }
            Poly composed = PolyCompose(&new, k, tab);
            push(p, &composed);
            PolyDestroy(&new);
            for (unsigned long i = 0; i < k; i++) {
                PolyDestroy(&tab[i]);
            }
            free(tab);
        }
    } else if (startsWith(s, "COMPOSE")) {
        if (isWhite(s + 7))*error = ERROR_COMPOSE;
        else *error = ERROR_WRONG_COMMAND;
    } else {
        *error = ERROR_WRONG_COMMAND;
    }
}

void scanOperation(char *s, struct Stack *p, int *error) {
    Poly new;
    if (startsWith(s, "ZERO")) getAndPush(p, PolyZero);
    else if (startsWith(s, "IS_COEFF")) topAndPrint(p, PolyIsCoeff, error);
    else if (startsWith(s, "IS_ZERO")) topAndPrint(p, PolyIsZero, error);
    else if (startsWith(s, "CLONE")) oneArg(p, PolyClone, error, 0);
    else if (startsWith(s, "ADD")) twoArgs(p, PolyAdd, error);
    else if (startsWith(s, "MUL")) twoArgs(p, PolyMul, error);
    else if (startsWith(s, "NEG")) oneArg(p, PolyNeg, error, 1);
    else if (startsWith(s, "SUB")) twoArgs(p, PolySub, error);
    else if (startsWith(s, "IS_EQ")) {
        if (p->size <= 1) *error = ERROR_STACK_UNDERFLOW;
        else {
            Poly first = pop(p);
            Poly second = top(p);
            printf(PolyIsEq(&first, &second) ? "1\n" : "0\n");
            push(p, &first);
        }
    } else if (startsWith(s, "AT ")) {
        char *endptr;
        errno = 0;
        poly_coeff_t x = strtol(s + 3, &endptr, 10);
        if (errno != 0 || *(s + 3) == '+' || isWhite(s + 3)) {
            *error = ERROR_AT;
            return;
        } else if (*endptr != -1 && *endptr != '\n' && *endptr != 0) {
            *error = ERROR_AT;
            return;
        }
        if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
        else {
            Poly top = pop(p);
            new = PolyAt(&top, x);
            push(p, &new);
            PolyDestroy(&top);
        }
    } else if (startsWith(s, "AT")) {
        if (isWhite(s + 2))*error = ERROR_AT;
        else *error = ERROR_WRONG_COMMAND;
    } else if (startsWith(s, "PRINT")) {
        if (p->size == 0) *error = ERROR_STACK_UNDERFLOW;
        else {
            new = top(p);
            PrintPoly(&new);
            printf("\n");
        }
    } else {
        scanIrregularOperation(s, p, error);
    }
}

void printError(int error, int ind) {
    if (error == ERROR_WRONG_COMMAND)fprintf(stderr, "ERROR %d WRONG COMMAND\n", ind);
    else if (error == ERROR_STACK_UNDERFLOW)fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", ind);
    else if (error == ERROR_WRONG_POLY)fprintf(stderr, "ERROR %d WRONG POLY\n", ind);
    else if (error == ERROR_DEG_BY)fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", ind);
    else if (error == ERROR_AT)fprintf(stderr, "ERROR %d AT WRONG VALUE\n", ind);
    else if (error == ERROR_COMPOSE)fprintf(stderr, "ERROR %d COMPOSE WRONG PARAMETER\n", ind);
}

Poly badPoly(int *error, struct Vector *v) {
    *error = ERROR_WRONG_POLY;
    if (v != NULL) {
        for (size_t i = 0; i < v->size; i++)PolyDestroy(&v->arr[i].p);
        free(v->arr);
    }
    return PolyZero();
}

/**
 * Wczytuje wielomian z linii zakładając, że składa się z jednomianów.
 * @param[in] s : linia
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 * @return wczytany wielomian
 */
static Poly scanMonos(char **s, int *error) {
    struct Vector v;
    initVector(&v);
    Mono m;
    while (**s) {
        if (**s == '(')(*s)++;
        else return badPoly(error, &v);
        Poly new = scanPoly(s, error);
        if (**s == ',')(*s)++; ///ominiecie przecinka
        else {
            PolyDestroy(&new);
            return badPoly(error, &v);
        }
        char *endptr;
        errno = 0;
        long exp_long = strtoll(*s, &endptr, 10);
        /// zakładam, że -0 nie jest dobrym wykładnikiem
        if (errno != 0 || exp_long > EXP_MAX || !is_digit(*s) || *endptr != ')') {
            PolyDestroy(&new);
            return badPoly(error, &v);
        }
        poly_exp_t exp = (poly_exp_t) exp_long;
        *s = endptr;
        if (!PolyIsZero(&new)) {
            m = MonoFromPoly(&new, exp);
            addToVector(&v, &m);
        }
        if (**s == ')')(*s)++; ///przejscie po zamykajacym nawiasie
        else {
            return badPoly(error, &v);
        }
        if (**s == '+')(*s)++; ///przejscie po plusie
        else break; ///jak nie ma plusa to konczymy wielomian; jesli czegos nie przerobilismy to potem to zobaczymy
    }
    Poly res = PolyAddMonos(v.size, v.arr);
    free(v.arr);
    return res;
}

/**
 * Wczytuje wielomian z linii zakładając, że jest współczynnikiem.
 * @param[in] s : linia
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 * @return wczytany wielomian
 */
static Poly scanCoeff(char **s, int *error) {
    char *endptr;
    errno = 0;
    poly_coeff_t x = strtol(*s, &endptr, 10);
    if (errno != 0 || (!is_digit(*s) && **s != '-')) return badPoly(error, NULL);
    *s = endptr;
    return PolyFromCoeff(x);
}

Poly scanPoly(char **s, int *error) {
    if (**s == '(') return scanMonos(s, error);
    else return scanCoeff(s, error);
}
