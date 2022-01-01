/** @file
 Interfejs kalkulatora wielamianów wielu zmiennych.
 @author Jan Wojtach
 @date 2021
 */
#ifndef POLYNOMIALS_CALCFUNCTIONS_H
#define POLYNOMIALS_CALCFUNCTIONS_H

/**
 * identyfikator błędu wrong command
 */
#define ERROR_WRONG_COMMAND 1

/**
 * identyfikator błędu stack underflow
 */
#define ERROR_STACK_UNDERFLOW 2

/**
 * identyfikator błędu wrong poly
 */
#define ERROR_WRONG_POLY 3

/**
 * identyfikator błędu wrong deg by
 */
#define ERROR_DEG_BY 4

/**
 * identyfikator błędu wrong at
 */
#define ERROR_AT 5

/**
 * identyfikator błędu wrong compose
 */
#define ERROR_COMPOSE 6

/**
 * Sprawdza, czy znak jest biały.
 * @param[in] c : sprawdzany znak
 * @return znak_jest_biały
 */
int isWhite(const char *c);

/**
 * Sprawdza, czy linia zaczyna się od litery.
 * @param[in] s : linia
 * @return linia_zaczyna_się_literą
 */
int startsWithLetter(char *s);

/**
 * Sprawdza, czy linia jest komentarzem (zaczyna się hashem).
 * @param[in] s : linia
 * @return linia_jest_komentarzem
 */
int isComment(const char *s);

/**
 * Sprawdza, czy w linii znajduje się \0 (w środku linii).
 * @param[in] s : linia
 * @param[in] ind : długość linii
 * @return linia_ma_\0
 */
int hasNull(char *s, size_t ind);

/**
 * Sprawdza, czy linia jest pusta.
 * @param[in] s : linia
 * @param[in] size : rozmiar
 * @return linia_jest_pusta
 */
int isEmpty(char *s, int size);

/**
 * Sprawdza, czy linie są takie same (lub jeśli linia ma zawierać argument to sprawdza, czy druga linia jest prefiksem pierwszej).
 * @param[in] s1 : linia
 * @param[in] s2 : wzorzec
 * @return s1==s2+x
 */
int startsWith(char *s1, char *s2);

/**
 * Wykonuje operację na podstawie linii.
 * @param[in] s : linia
 * @param[in] p : Stos, na którym potencjalnie wykona się operacja
 * @param[in] error : wskaźnik na miejsce, w którym przechowywane są informacje o błędach
 */
void scanOperation(char *s, struct Stack *p, int *error);

/**
 * Wypisuje błąd na podstawie informacji.
 * @param[in] error : kod błędu
 * @param[in] ind : numer wiersza
 */
void printError(int error, int ind);

/**
 * Zaznacza, że na wejściu został podany błędny wielomian i czyści pamięć poświęconą na tworzenie wielomianu.
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 * @param[in] v : wektor, który trzeba wyczyścić
 * @return wielomian-atrapa (potrzebny, by zwracana wartość się zgadzała, jednak nigdy się nie przyda)
 */
Poly badPoly(int *error, struct Vector *v);

/**
 * Wczytuje wielomian z linii
 * @param[in] s : wskaźnik na linię
 * @param[in] error : wskaźnik na miejsce przechowujące kod błędu
 * @return wczytany wielomian (lub wielomian-atrapa w przypadku błędu)
 */
Poly scanPoly(char **s, int *error);

#endif //POLYNOMIALS_CALCFUNCTIONS_H
