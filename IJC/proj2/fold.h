/** @file fold.h
 * @brief Rozhranie pre fold.c
 * Riesenie IJC-DU2, priklad a)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */
#ifndef FOLD_H
#define FOLD_H

#include <string.h>
#include <stdlib.h>
#include "io.h"


#define FILENAME_MAX_LENGTH 255 /* Maximalna dlzka mena suboru */
#define WORD_MAX_LENGTH 4096 /* Maximalna dlzka slova */
typedef struct
{
    FILE *file; 
    int width;
    bool c;
}  optionsStruct;
void vypis(optionsStruct options);

#endif /* !FOLD_H */
