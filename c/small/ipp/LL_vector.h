#ifndef LL_VECTOR_H
#define LL_VECTOR_H
struct LL_vector {
    unsigned long long *longLongs;
    int allocated;
    int elements;
};

void init_LL_vector(struct LL_vector *v);

void add_to_LL_vector(struct LL_vector *v, unsigned long long x);

int are_LL_vectors_equal(struct LL_vector *v1, struct LL_vector *v2);

#endif //UNTITLED1_LL_VECTOR_H
