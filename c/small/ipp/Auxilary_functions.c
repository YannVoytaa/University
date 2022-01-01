#include "Auxilary_functions.h"
#include <string.h>

#define LOWER_ASCII_LIMIT (char)33
#define UPPER_ASCII_LIMIT (char)126
#define LOWER_WHITE_LIMIT (char)9
#define UPPER_WHITE_LIMIT (char)13
#define SPACE 32
#define HASH '#'

int is_ascii(const char *c) {
    return (*c >= LOWER_ASCII_LIMIT && *c <= UPPER_ASCII_LIMIT);
}

int is_white(const char *c) {
    return (*c == SPACE || (*c >= LOWER_WHITE_LIMIT && *c <= UPPER_WHITE_LIMIT));
}

int not_valid(char *s, int size) {
    for (int i = 0; i < size; i++) {
        if (!is_ascii(s) && !is_white(s)) return 1;
        s++;
    }
    return 0;
}

int is_comment(const char *s) {
    return (*s == HASH);
}

int is_empty(char *s, int size) {
    for (int i = 0; i < size; i++) {
        if (!is_white(s)) return 0;
        s++;
    }
    return 1;
}

int has_x(char *s) {
    while (*s) {
        //zakladamy ze wszystkie litery sa juz zmienione na male
        if (*s == 'x')return 1;
        s++;
    }
    return 0;
}

int cmp_ll(const void *a, const void *b) {
    if (*(unsigned long long *) a > *(unsigned long long *) b)return 1;
    else if (*(unsigned long long *) a < *(unsigned long long *) b) return -1;
    else return 0;
}

int cmp_ld(const void *a, const void *b) {
    if (*(long double *) a > *(long double *) b)return 1;
    else if (*(long double *) a < *(long double *) b) return -1;
    else return 0;
}

int cmp_str(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}