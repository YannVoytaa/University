/** @file
 Implementacja klasy dynamicznych tablic.
 @author Jan Wojtach
 @date 2021
 */

#include <malloc.h>
#include <stdlib.h>
#include "Vector.h"
/**
 * Domyslny rozmiar dynamicznej tablicy
 */
#define INIT_SIZE 4

void addCopyToVector(struct Vector *v, const Mono *m) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->arr = realloc(v->arr, v->capacity * sizeof(Mono));
        if (v->arr == NULL) exit(1);
    }
    v->arr[v->size] = MonoClone(m);
    (v->size)++;
}

void addToVector(struct Vector *v, const Mono *m) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->arr = realloc(v->arr, v->capacity * sizeof(Mono));
        if (v->arr == NULL) exit(1);
    }
    v->arr[v->size] = *m;
    (v->size)++;
}

void initVector(struct Vector *v) {
    initVectorWithCount(v, INIT_SIZE);
}

void initVectorWithCount(struct Vector *v, size_t count) {
    v->arr = calloc(count, sizeof(struct Mono));
    if (v->arr == NULL) exit(1);
    v->size = 0;
    v->capacity = count;
}