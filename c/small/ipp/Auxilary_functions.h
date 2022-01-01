#ifndef AUXILARY_FUNCTIONS_H
#define AUXILARY_FUNCTIONS_H

int is_ascii(const char *c);

int is_white(const char *c);

int not_valid(char *s, int size);

int is_comment(const char *s);

int is_empty(char *s, int size);

//funkcja sprawdza, czy w slowie istnieje litera x
//pomoze nam to w tym, aby strtold nie uznawal liczb szesnastkowych za poprawne;
//chcemy bowiem, zeby przed liczbami szesnastkowymi nie bylo znaku +, a w ogolnosci
//dla liczb zmiennoprzecinkowych moze to zachodzic
int has_x(char *s);

//funkcje z przedrostkiem cmp_ porownuja elementy pewnego typu
//i sa podawane jako argument qsortow
int cmp_ll(const void *a, const void *b);

int cmp_ld(const void *a, const void *b);

int cmp_str(const void *a, const void *b);

#endif //AUXILARY_FUNCTIONS_H
