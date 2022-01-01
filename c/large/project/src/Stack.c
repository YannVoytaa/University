/** @file
 Implementacja klasy stosów.
 @author Jan Wojtach
 @date 2021
 */

#include <stdlib.h>
#include "Stack.h"
#include "poly.h"
/**
 * Domyslny rozmiar dynamicznej tablicy
 */
#define INIT_SIZE 4

void push(struct Stack *v, Poly *m) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->arr = realloc(v->arr, v->capacity * sizeof(Poly));
        if (v->arr == NULL) exit(1);
    }
    v->arr[v->size] = *m;
    (v->size)++;
}

void initStack(struct Stack *v) {
    initStackWithCount(v, INIT_SIZE);
}

void destroyStack(struct Stack *v) {
    Poly from_top;
    while (v->size != 0) {
        from_top = pop(v);
        PolyDestroy(&from_top);
    }
    free(v->arr);
}

void initStackWithCount(struct Stack *v, size_t count) {
    v->arr = calloc(count, sizeof(Poly));
    if (v->arr == NULL) exit(1);
    v->size = 0;
    v->capacity = count;
}

Poly top(struct Stack *v) {
    return v->arr[v->size - 1];
}

Poly pop(struct Stack *v) {
    Poly res = v->arr[v->size - 1];
    v->size--;
    return res;
}