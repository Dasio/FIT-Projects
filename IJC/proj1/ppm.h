/** @file ppm.h
* @brief Implementacia rozhrania pre ppm.c
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 3.3.2014
*/
#ifndef PPM_H
#define PPM_H
#include "error.h"
#include <ctype.h>
/** @struct ppm
 *  @brief Struktura pre uchovanie dat pre PPM format.
 *  @var ppm::xsize 
 *  'xsize' obsahuje rozmer x-ovej osi.
 *  @var ppm::ysize 
 *  'ysize' obsahuje rozmer y-ovej osi.
 *  @var ppm::data[]
 *  'data[]' obsahuju RGB bajty, celkom 3*xsize*ysize.
 */
struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];    
};

/**
 * @brief Zapise data zo stuktury do suboru.
 * @param filename Nazov suboru.
 * @return Ak sa podaril zapas tak 0, inak -1.
 */
struct ppm * ppm_read(const char *filename);

 /**
 * @brief Nacita data zo suboru do struktury.
 * @param filename Nazov suboru.
 * @return Ukazatel na ppm strukturu, v pripade chyby NULL.
 */
int ppm_write(struct ppm *p, const char *filename);
#endif /* !PPM_H */
