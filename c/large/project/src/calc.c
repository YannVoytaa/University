/** @file
 Obsługa wejścia kalkulatora wielamianów wielu zmiennych.
 @author Jan Wojtach
 @date 2021
 */
///bez ponizszego makro wystepuje warning dla getline
#define  _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <errno.h>
#include "poly.h"
#include "Stack.h"
#include "Vector.h"
#include "CalcFunctions.h"
/**
 * Domyślna długość linii
 */
#define INITIAL_BUFSIZE 32

int main() {
    size_t bufsize = INITIAL_BUFSIZE;
    int line_ind = 0;
    char *line = (char *) malloc(bufsize * sizeof(char));
    char *zapas;
    if (line == NULL)exit(1);
    ssize_t line_size;
    struct Stack p;
    initStack(&p);
    int error = 0;
    while ((line_size = getline(&line, &bufsize, stdin)) && line_size != -1) {
        line_ind++;
        if (isComment(line) || isEmpty(line, line_size)) {
            continue;
        } else {
            error = 0;
            if (startsWithLetter(line)) {
                if (hasNull(line, line_size)) {
                    if (startsWith(line, "DEG_BY "))printError(ERROR_DEG_BY, line_ind);
                    else if (startsWith(line, "AT "))printError(ERROR_AT, line_ind);
                    else if (startsWith(line, "COMPOSE "))printError(ERROR_COMPOSE, line_ind);
                    else printError(ERROR_WRONG_COMMAND, line_ind);
                    continue;
                }
                scanOperation(line, &p, &error);
                printError(error, line_ind);
            } else {
                if (hasNull(line, line_size)) {
                    printError(ERROR_WRONG_POLY, line_ind);
                    continue;
                }
                zapas = malloc((line_size + 1) * sizeof(char));
                if (zapas == NULL) exit(1);
                strcpy(zapas, line);
                char **endptr = &zapas;
                char *start = zapas;
                Poly new = scanPoly(endptr, &error);
                if (**endptr != '\n' && **endptr != -1 && **endptr != 0)
                    error = ERROR_WRONG_POLY; //nie dotarliśmy do końca linii
                printError(error, line_ind);
                if (error == 0) push(&p, &new);
                else PolyDestroy(&new);
                free(start);
            }
        }
    }
    if (line == NULL || errno == ENOMEM) exit(1);
    destroyStack(&p);
    free(line);
}