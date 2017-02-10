/** @file htab_remove.c
 * @brief Vyhlada polozku v hashtable, upravi poradie a zmaze ju
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"

void htab_remove(htab_t *t,const char *key)
{
	if (t==NULL || key==NULL) return;
	/* Spravene zalohy klucu, pre pripad ze sa s ukazatelom v priebehu nieco stane */
	char *keyBackup=malloc(strlen(key)+1);
	if(keyBackup==NULL)
	{
		fprintf(stderr,"Nepodarilo sa spravit zalohu kluca");
		return;
	}
	unsigned index = hash_function(key,t->size);
	struct htab_listitem *item = t->data[index];
	struct htab_listitem *itemP = NULL;
	/* Prejdeme cely zoznam */
	while (item)
	{
		/* Ak sme nasli kluc */
		if(strcmp(item->key,keyBackup)==0)
		{
			/* Ak ma predchodcu aj nasledovnika */
			if (itemP && item->next)
				itemP->next = item->next;
			/* Nema predchodcu, ma nasledovnika */
			else if ( !itemP && item->next)
				t->data[index] = item->next;
			/* Ma predchodcu, nema nasledovnika */
			else if (!item->next && itemP)
				itemP->next = NULL;
			/* Je jediny na danom indexe */
			else
				t->data[index] = NULL;
			hitem_free(item);
			/* V hashtably by nemali byt duplikaty, mozme ukoncit */
			free(keyBackup);
			break;
		}
		itemP=item;
		item=item->next;
	}
	free(keyBackup);
}
