//bez ponizszego makro wystepuje warning dla getline
#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include "Wiersz.h"
#include "Wiersz_vector.h"
#include "Auxilary_functions.h"

#define INITIAL_BUFSIZE 32

int main() {
    size_t bufsize = INITIAL_BUFSIZE;
    int line_ind = 0;
    char *line = (char *) malloc(bufsize * sizeof(char));
    if (line == NULL)exit(1);
    char *word = (char *) malloc(bufsize * sizeof(char));
    if (word == NULL)exit(1);
    struct Wiersz_vector wv;
    init_Wiersz_vector(&wv);
    int line_size;
    while ((line_size = getline(&line, &bufsize, stdin)) && line_size != -1) {
        if (line == NULL) exit(1);
        line_ind++;
        if (is_comment(line) || is_empty(line, line_size)) {
            continue;
        } else if (not_valid(line, line_size)) {
            fprintf(stderr, "ERROR %d\n", line_ind);
        } else {
            add_empty_Wiersz(&wv);
            //uzupelniamy wiersz danymi, ktore beda go identyfikowaly
            wv.wierszs[wv.elements - 1].id = line_ind;
            wv.wierszs[wv.elements - 1].next_similar = 0;
            wv.wierszs[wv.elements - 1].pos = wv.elements - 1;
            Wiersz_from_string(line, word, &wv.wierszs[wv.elements - 1]);
        }
    }
    // sortujemy wiersze, przez co wiersze "podobne" znajda sie obok siebie
    // (podobne wiersze beda posortowane wg id, czyli swojego numeru)
    qsort(wv.wierszs, wv.elements, sizeof(struct Wiersz), cmp_Wiersz);
    //gdy wiersze sa posortowane to interesuja nas tylko relacje miedzy sasiadujacymi wierszami
    for (int i = 0; i < wv.elements - 1; i++) {
        if (compare_Wierszs(&wv.wierszs[i], &wv.wierszs[i + 1]))wv.wierszs[i].next_similar = wv.wierszs[i + 1].pos;
    }
    //wracamy do ustawienia wierszy rosnaco po numerze, zeby wypisac podobne wiersze w pozadanej kolejnosci
    qsort(wv.wierszs, wv.elements, sizeof(struct Wiersz), cmp_Wiersz_id);
    print_similar_lines(&wv);
    free(line);
    free(word);
    for (int i = 0; i < wv.elements; i++) {
        free_Wiersz(&wv.wierszs[i]);
    }
    free(wv.wierszs);
}