#include <stdio.h>
#include <stdlib.h>
#include "LL_vector.h"

#define INITIAL_VECTOR_SIZE 4

void init_LL_vector(struct LL_vector *v) {
    v->longLongs = (unsigned long long *) malloc(INITIAL_VECTOR_SIZE*sizeof(unsigned long long));
    if (v->longLongs == NULL)exit(1);
    v->allocated = INITIAL_VECTOR_SIZE;
    v->elements = 0;
}

void add_to_LL_vector(struct LL_vector *v, unsigned long long x) {
    if (v->allocated == v->elements) {
        v->longLongs = (unsigned long long *) realloc(v->longLongs, 2 * (v->allocated) * sizeof(unsigned long long));
        v->allocated *= 2;
    }
    v->longLongs[v->elements] = x;
    (v->elements)++;
}

int are_LL_vectors_equal(struct LL_vector *v1, struct LL_vector *v2) {
    if (v1->elements != v2->elements) return 0;
    //zakladamy, ze wektory sa posortowane
    for (int i = 0; i < v1->elements; i++) {
        if (v1->longLongs[i] != v2->longLongs[i]) return 0;
    }
    return 1;
}