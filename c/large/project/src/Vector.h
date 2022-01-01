/** @file
 Interfejs klasy dynamicznych tablic.
 @author Jan Wojtach
 @date 2021
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "poly.h"

/**
 * To jest struktura przechowująca dynamiczną tablicę jednomianów.
 */
struct Vector {
    /**
     * Tablica jednomianów.
     */
    Mono *arr;
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
 * Dodaje kopię jednomianu na koniec tablicy (i w razie konieczności rozszerza tablicę).
 * @param[in] v : tablica v
 * @param[in] m : jednomian, którego kopię chcemy dodać
 */
void addCopyToVector(struct Vector *v, const Mono *m);

/**
 * Dodaje jednomian na koniec tablicy (i w razie konieczności rozszerza tablicę).
 * @param[in] v : tablica v, do której chcemy dodać jednomian
 * @param[in] m : jednomian, który chcemy dodać
 */
void addToVector(struct Vector *v, const Mono *m);

/**
 * Tworzy tablicę o pewnej stałej wielkości.
 * @param[in] v : wskaźnik na tablicę, którą mamy zamiar zainicjalizować
 */
void initVector(struct Vector *v);

/**
 * Tworzy tablicę wielkości count.
 * @param[in] v : wskaźnik na tablicę, którą mamy zamiar zainicjalizować
 * @param[in] count : wielkość tablicy
 */
void initVectorWithCount(struct Vector *v, size_t count);

#endif //VECTOR_H
