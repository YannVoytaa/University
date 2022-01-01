#include <string.h>
#include <stdlib.h>
#include "String_vector.h"

#define INITIAL_VECTOR_SIZE 4

void init_String_vector(struct String_vector *v) {
    v->strings = (char **) malloc(INITIAL_VECTOR_SIZE*sizeof(char *));
    if (v->strings == NULL)exit(1);
    v->allocated = INITIAL_VECTOR_SIZE;
    v->elements = 0;
}

void make_String_lowerCase(char *s) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')s[i] += 'a' - 'A';
    }
}

void add_to_String_vector(struct String_vector *v, char *s) {
    if (v->allocated == v->elements) {
        v->strings = (char **) realloc(v->strings, 2 * (v->allocated) * sizeof(char *));
        v->allocated *= 2;
    }
    v->strings[v->elements] = malloc((strlen(s) + 1) * sizeof(char));
    if (v->strings[v->elements] == NULL)exit(1);
    strcpy(v->strings[v->elements], s);
    make_String_lowerCase(v->strings[v->elements]);
    (v->elements)++;
}

int are_String_vectors_equal(struct String_vector *v1, struct String_vector *v2) {
    if (v1->elements != v2->elements) return 0;
    //zakladamy, ze wektory sa posortowane
    for (int i = 0; i < v1->elements; i++) {
        if (strcmp(v1->strings[i], v2->strings[i]) != 0) return 0;
    }
    return 1;
}

void free_string_vector(struct String_vector *v) {
    for (int i = 0; i < v->elements; i++)free(v->strings[i]);
    free(v->strings);
}