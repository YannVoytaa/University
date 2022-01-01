#include <stdio.h>
#include <stdlib.h>
#include "LD_vector.h"

#define INITIAL_VECTOR_SIZE 4

void init_LD_vector(struct LD_vector *v) {
    v->longDoubles = (long double *) malloc(INITIAL_VECTOR_SIZE*sizeof(long double));
    if (v->longDoubles == NULL)exit(1);
    v->allocated = INITIAL_VECTOR_SIZE;
    v->elements = 0;
}

void add_to_LD_vector(struct LD_vector *v, long double x) {
    if (v->allocated == v->elements) {
        v->longDoubles = (long double *) realloc(v->longDoubles, 2 * (v->allocated) * sizeof(long double));
        v->allocated *= 2;
    }
    v->longDoubles[v->elements] = x;
    (v->elements)++;
}

int are_LD_vectors_equal(struct LD_vector *v1, struct LD_vector *v2) {
    if (v1->elements != v2->elements) return 0;
    //zakladamy, ze wektory sa posortowane
    //w takim razie jesli sa rowne, to sa rowne na odpowiadajacych pozycjach
    for (int i = 0; i < v1->elements; i++) {
        if (v1->longDoubles[i] != v2->longDoubles[i]) return 0;
    }
    return 1;
}