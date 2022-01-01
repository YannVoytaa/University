#include <stdio.h>
#include <stdlib.h>
#include "Wiersz_vector.h"
#include "Wiersz.h"

#define INITIAL_VECTOR_SIZE 4

void init_Wiersz_vector(struct Wiersz_vector *v) {
    v->wierszs = (struct Wiersz *) malloc(INITIAL_VECTOR_SIZE*sizeof(struct Wiersz));
    if (v->wierszs == NULL)exit(1);
    v->allocated = INITIAL_VECTOR_SIZE;
    v->elements = 0;
}

void add_empty_Wiersz(struct Wiersz_vector *v) {
    if (v->allocated == v->elements) {
        v->wierszs = (struct Wiersz *) realloc(v->wierszs, 2 * (v->allocated) * sizeof(struct Wiersz));
        v->allocated *= 2;
    }
    (v->elements)++;
}

void print_similar_lines(struct Wiersz_vector *w) {
    int ind;
    for (int i = 0; i < w->elements; i++) {
        ind = i;
        //szukamy wszystkich podobnych do itego, chyba ze juz byl wypisywany
        while (w->wierszs[ind].id != -1) {
            printf("%d", w->wierszs[ind].id);
            //zaznaczamy, ze juz zapisalismy ten wiersz
            //pozniej juz go nie zapiszemy
            w->wierszs[ind].id = -1;
            ind = w->wierszs[ind].next_similar;
            //jesli wiersz nie wskazywal na nastepny to nie ma wiecej podobnych wierszow
            if (ind == 0) {
                printf("\n");
                break;
            } else printf(" ");
        }
    }
}