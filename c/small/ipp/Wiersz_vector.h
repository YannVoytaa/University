#ifndef WIERSZ_VECTOR_H
#define WIERSZ_VECTOR_H
struct Wiersz_vector {
    struct Wiersz *wierszs;
    int allocated;
    int elements;
};

void init_Wiersz_vector(struct Wiersz_vector *v);

void add_empty_Wiersz(struct Wiersz_vector *v);

//tworzy output na podstawie relacji miedzy wierszami
void print_similar_lines(struct Wiersz_vector *w);

#endif //Wiersz_vector_H
