/** @file htab_clear.c
 * @brief Vymaze a uvolni kazdu polozku v hashovacej tabulke
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"

void htab_clear(htab_t *t)
{
	if (t==NULL) return;
	struct htab_listitem *item = NULL;
	struct htab_listitem *temp = NULL;
	for (unsigned long i = 0; i<t->size; i++)
	{
		if (t->data[i]==NULL) continue;
		for(item=t->data[i];item;item=temp)
		{
			/* Docasne ulozit, aby sme sa vedeli po uvolneni dalej pohybovat v zozname */
			temp=item->next;
			hitem_free(item);
		}
		t->data[i] = NULL;
	}
	
}
