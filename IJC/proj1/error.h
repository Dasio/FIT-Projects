/** @file error.h
* @brief Implementacia rozhrania pre error.c
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 3.3.2014
*/
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * @brief Vypise chybu.
 * @param fmt Format vystupu.
 * @return Void.
 */
void Warning(const char *fmt, ...);

/**
 * @brief Vypise chybu a ukonci cely program.
 * @param fmt Format vystupu.
 * @return Void.
 */
void FatalError(const char *fmt, ...);
#endif /* !ERROR_H */
