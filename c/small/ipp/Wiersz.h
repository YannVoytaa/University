#ifndef WIERSZ_H

#include "LL_vector.h"
#include "LD_vector.h"
#include "String_vector.h"

#define WIERSZ_H
struct Wiersz {
    struct LD_vector long_doubles; //liczby ujemne i zmiennoprzecinkowe niecalkowite
    struct LL_vector long_longs; //liczby nieujemne calkowite
    struct String_vector words_and_long_numbers;
    int id;
    int next_similar;
    int pos;
};

void init_Wiersz(struct Wiersz *v);

int compare_Wierszs(struct Wiersz *v1, struct Wiersz *v2);

void Wiersz_from_string(char *line, char *word, struct Wiersz *g);

//porownuje wiersze tak, ze podobne wiersze roznia sie najpozniej, czyli beda obok siebie
int cmp_Wiersz(const void *a, const void *b);

//porownuje wiersze po ich id (numerach wejscia)
int cmp_Wiersz_id(const void *a, const void *b);

void free_Wiersz(struct Wiersz *g);

#endif //WIERSZ_H
