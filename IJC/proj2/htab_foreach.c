/** @file htab_foreach.c
 * @brief Nad kazdym prvkom hashovacej tabulky spusti zadanu funkciu
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"
void htab_foreach(htab_t *t,void (*fp)(const char*,unsigned))
{
	if (t==NULL) return;
	struct htab_listitem * item = NULL;
	for (unsigned long i=0;i<t->size;i++)
	{
		/* Na danom indexe neni ani jeden prvok, preskocit */
		if(t->data[i]==NULL) continue;
		item = t->data[i];
		/* Prejdenie celeho zoznamu */
		while(item)
		{
			fp(item->key,item->data);
			item = item->next;
		}
	}
}
