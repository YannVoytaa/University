#ifndef LD_VECTOR_H
#define LD_VECTOR_H
struct LD_vector {
    long double *longDoubles;
    int allocated;
    int elements;
};

void init_LD_vector(struct LD_vector *v);

void add_to_LD_vector(struct LD_vector *v, long double x);

int are_LD_vectors_equal(struct LD_vector *v1, struct LD_vector *v2);

#endif //LD_VECTOR_H
