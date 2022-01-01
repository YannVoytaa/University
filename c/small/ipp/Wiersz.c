#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "Wiersz.h"
#include "String_vector.h"
#include "LD_vector.h"
#include "LL_vector.h"
#include "Auxilary_functions.h"

void init_Wiersz(struct Wiersz *v) {
    init_LD_vector(&v->long_doubles);
    init_LL_vector(&v->long_longs);
    init_String_vector(&v->words_and_long_numbers);
}

void put_into_Wiersz_as_string(struct Wiersz *v, char *s) {
    add_to_String_vector(&v->words_and_long_numbers, s);
}

int put_into_Wiersz_as_ll(struct Wiersz *v, char *s) {
    char *pEnd;
    //wyjatek- liczba 0x=0, ale strtoul uzna, ze to nieliczba
    //a do strtold nie wpuszczamy liczb szesnastkowych
    // (bo nie moga miec + na poczatku, a liczby zminnoprzecinkowe moga)
    if (s[0] == '0' && s[1] == 'x') {
        if (s[2] == '\0') {
            add_to_LL_vector(&v->long_longs, 0);
            return 1;
        }
    }
    if (s[0] == '-') {
        errno = 0;
        long long x = strtoll(s, &pEnd, 10);
        if (*pEnd != '\0' || errno != 0) {
            return 0;
        } else {
            //w v->long_longs trzymamy nieujemne liczby calkowite,
            //a w long_doubles ujemne i zmiennoprzecinkowe niecalkowite
            if (x == 0)add_to_LL_vector(&v->long_longs, x);
            else add_to_LD_vector(&v->long_doubles, x);
            return 1;
        }
    } else {
        unsigned long long x;
        errno = 0;
        if (s[0] == '+')x = strtoul(s, &pEnd, 10);
        else x = strtoul(s, &pEnd, 0);
        if (*pEnd != '\0' || errno != 0) {
            return 0;
        } else {
            add_to_LL_vector(&v->long_longs, x);
            return 1;
        }
    }
}

int put_into_Wiersz_as_lld(struct Wiersz *v, char *s) {
    char *pEnd;
    errno = 0;
    long double x = strtold(s, &pEnd);
    //warunek x!=x odnosi sie do wartosci nan: jesli wystepuje to traktujemy
    //ja jako nieliczbe; has_x- wytlumaczone wczesniej, nie chcemy liczb szesnastkowych w strtold
    if (*pEnd != '\0' || errno != 0 || x != x || has_x(s)) {
        return 0;
    } else {
        //jesli liczba jest calkowita i nieujemna to chcemy ja trzymac z pozostalymi calkowitymi nieujemnymi
        if (x >= 0 && x == (unsigned long long) x)add_to_LL_vector(&v->long_longs, (unsigned long long) x);
        else add_to_LD_vector(&v->long_doubles, x);
        return 1;
    }
}

void add_to_Wiersz(struct Wiersz *v, char *s) {
    int udane = put_into_Wiersz_as_ll(v, s);
    if (udane == 0) udane = put_into_Wiersz_as_lld(v, s);
    if (udane == 0) put_into_Wiersz_as_string(v, s);
}

int compare_Wierszs(struct Wiersz *v1, struct Wiersz *v2) {
    if (are_String_vectors_equal(&v1->words_and_long_numbers, &v2->words_and_long_numbers)
        && are_LD_vectors_equal(&v1->long_doubles, &v2->long_doubles)
        && are_LL_vectors_equal(&v1->long_longs, &v2->long_longs))
        return 1;
    else return 0;
}

void take_words_and_put_into_Wiersz(char *line, char *word, struct Wiersz *v) {
    do {
        while (is_white(line))line++;
        if (*line == '\0')break;
        sscanf(line, "%s", word);
        add_to_Wiersz(v, word);
        line += strlen(word) + 1;
    } while (*word && *line);
}

void Wiersz_from_string(char *line, char *word, struct Wiersz *g) {
    init_Wiersz(g);
    take_words_and_put_into_Wiersz(line, word, g);
    qsort(g->long_longs.longLongs, g->long_longs.elements, sizeof(unsigned long long), cmp_ll);
    qsort(g->long_doubles.longDoubles, g->long_doubles.elements, sizeof(long double), cmp_ld);
    qsort(g->words_and_long_numbers.strings, g->words_and_long_numbers.elements, sizeof(char *), cmp_str);
}

int cmp_Wiersz(const void *a, const void *b) {
    struct Wiersz w1 = *(struct Wiersz *) a;
    struct Wiersz w2 = *(struct Wiersz *) b;
    //ustalamy wierszom pewien priorytet- sortujemy po romiarach wektorow, a potem po elementach
    if (w1.long_longs.elements != w2.long_longs.elements)
        return w1.long_longs.elements - w2.long_longs.elements;
    if (w1.words_and_long_numbers.elements != w2.words_and_long_numbers.elements)
        return w1.words_and_long_numbers.elements - w2.words_and_long_numbers.elements;
    if (w1.long_doubles.elements != w2.long_doubles.elements)
        return w1.long_doubles.elements - w2.long_doubles.elements;
    for (int i = 0; i < w1.long_longs.elements; i++) {
        if (cmp_ll(&w1.long_longs.longLongs[i], &w2.long_longs.longLongs[i]) != 0)
            return cmp_ll(&w1.long_longs.longLongs[i], &w2.long_longs.longLongs[i]);
    }
    for (int i = 0; i < w1.words_and_long_numbers.elements; i++) {
        if (cmp_str(&w1.words_and_long_numbers.strings[i], &w2.words_and_long_numbers.strings[i]) != 0)
            return cmp_str(&w1.words_and_long_numbers.strings[i], &w2.words_and_long_numbers.strings[i]);
    }
    for (int i = 0; i < w1.long_doubles.elements; i++) {
        if (cmp_ld(&w1.long_doubles.longDoubles[i], &w2.long_doubles.longDoubles[i]) != 0)
            return cmp_ld(&w1.long_doubles.longDoubles[i], &w2.long_doubles.longDoubles[i]);
    }
    return w1.id - w2.id;
}

int cmp_Wiersz_id(const void *a, const void *b) {
    struct Wiersz w1 = *(struct Wiersz *) a;
    struct Wiersz w2 = *(struct Wiersz *) b;
    return w1.id - w2.id;
}

void free_Wiersz(struct Wiersz *g) {
    free(g->long_doubles.longDoubles);
    free(g->long_longs.longLongs);
    free_string_vector(&g->words_and_long_numbers);
}