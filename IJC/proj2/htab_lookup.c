/** @file htab_lookup.c
 * @brief Vyhlada polozku v hashtable a zvysi data o 1, ak nenajde tak vytvori novu
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"

struct htab_listitem * htab_lookup(htab_t *t, const char *key)
{
	if (t==NULL || key==NULL) return NULL;
	/* Spravene zalohy klucu, pre pripad ze sa s ukazatelom v priebehu nieco stane */
	char *keyBackup=malloc(strlen(key)+1);
	if(keyBackup==NULL)
	{
		fprintf(stderr,"Nepodarilo sa spravit zalohu kluca");
		return NULL;
	}
	strcpy(keyBackup,key);
	/* Vypocitanie indexu kde je kluc ulozeny*/
	unsigned index = hash_function(keyBackup,t->size);
	struct htab_listitem * item = t->data[index];
	/* Pomocna premenna, aby sme neskor vedeli kde priradit novu polozku */
	struct htab_listitem * itemP = NULL;
	while(item)
	{
		if(strcmp(item->key,keyBackup)==0) 
		{
			free(keyBackup);
			return item;
		}
		itemP=item;
		item=item->next;
	}
	/* Kluc este neni v hastable */
	struct htab_listitem *newItem = hitem_init(keyBackup,0);
	if (newItem==NULL)
	{
		fprintf(stderr,"Nepodarilo sa vytvorit novu polozku\n");
		return NULL;
	}
	/* Ak sa newItem nema na co pripojit, tak sa priradi nazaciatok */
	if (itemP) itemP->next=newItem;
	else t->data[index] = newItem;
	free(keyBackup);
	return newItem;
}
