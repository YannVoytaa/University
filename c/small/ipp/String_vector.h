#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H
struct String_vector {
    char **strings;
    int allocated;
    int elements;
};

void init_String_vector(struct String_vector *v);

void add_to_String_vector(struct String_vector *v, char *s);

int are_String_vectors_equal(struct String_vector *v1, struct String_vector *v2);

void free_string_vector(struct String_vector *v);

#endif //STRING_VECTOR_H
