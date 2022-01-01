/** @file
 Interfejs klasy stosów.
 @author Jan Wojtach
 @date 2021
 */

#ifndef STACK_H
#define STACK_H

#include "Stack.h"
#include "poly.h"

/**
 * To jest struktura przechowująca dynamiczną tablicę wielomianów.
 */
struct Stack {
    /**
     * Tablica wielomianów.
     */
    Poly *arr;
    /**
     * Pojemność tablicy.
     */
    size_t capacity;
    /**
     * Ilość elementów w tablicy.
     */
    size_t size;
};

/**
 * Dodaje wielomian na koniec tablicy (i w razie konieczności rozszerza tablicę).
 * @param[in] v : tablica v, do której chcemy dodać jednomian
 * @param[in] m : wielomian, który chcemy dodać
 */
void push(struct Stack *v, Poly *m);

/**
 * Tworzy tablicę o pewnej stałej wielkości.
 * @param[in] v : wskaźnik na tablicę, którą mamy zamiar zainicjalizować
 */
void initStack(struct Stack *v);

/**
 * Tworzy tablicę wielkości count.
 * @param[in] v : wskaźnik na tablicę, którą mamy zamiar zainicjalizować
 * @param[in] count : wielkość tablicy
 */
void initStackWithCount(struct Stack *v, size_t count);

/**
 * Zwraca wielomian wrzucony jako ostatni
 * @param[in] v : stos
 * @return wielomian p
 */
Poly top(struct Stack *v);

/**
 * Zwraca wielomian z góry stosu i go zrzuca
 * @param[in] v : stos
 * @return wielomian p
 */
Poly pop(struct Stack *v);

/**
 * Usuwa stos z pamięci
 * @param[in] v : stos
 */
void destroyStack(struct Stack *v);

#endif //STACK_H
