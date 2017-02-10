/** @file io.h
 * @brief Rozhranie pre io.c
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#ifndef FGETW_H
#define FGETW_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * @brief Nacita slovo nasledujuce slovo, zo zadaneho suboru
 * @param s, buffer kde sa slovo ulozi
 * @param max, maximalna dlzka slova
 * @param f, subor odkial bude citat
 * @return Pocet nacitanych znakov, v pripade newlinu vracia 0
 */

int fgetw(char *s, int max, FILE *f);

#endif /* !FGETW_H */
