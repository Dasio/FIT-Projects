/** @file htab_free.c
 * @brief Uvolni celu hashtable
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"
void htab_free(htab_t *t)
{
	if (t==NULL) return;
	htab_clear(t);
	free(t);
	/* Zahodit ukazatel na hashtable, v tomto prototype funkcie neni mozny */
}
