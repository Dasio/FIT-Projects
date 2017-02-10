/** @file error.c
* @brief Modul pre vypis chyb.
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 3.3.2014
*/
#include "error.h"

void Warning(const char *fmt, ...)
{
    fprintf(stderr,"CHYBA: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr,fmt,args);
    va_end(args);    
}
void FatalError(const char *fmt, ...)
{
    fprintf(stderr,"CHYBA: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr,fmt,args);
    va_end(args);
    exit(1);
}
