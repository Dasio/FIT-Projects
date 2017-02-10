/** @file htab_function.c
 * @brief Hashovacia funkcia zo zadania
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "hash_function.h"
unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % htab_size;
}
