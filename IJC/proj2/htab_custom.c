/** @file htab_custom.c
 * @brief Funkcie ktore niesu zo zadania
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"

struct htab_listitem *hitem_init(const char *key,unsigned int data)
{
	if (key==NULL) return NULL;
	/* Alokuje miesto pre strukturu */
	struct htab_listitem * newItem = malloc(sizeof(*newItem));
	if (newItem==NULL) return NULL;
	/* Alokuje miesto pre nazov a koncovu nulu, strlen nepocita \0 */
	newItem->key = malloc(strlen(key)+1);
	if (newItem->key == NULL)
	{
		fprintf(stderr,"Nepodarilo sa alokovat miesto pre kluc");
		free(newItem);
		return NULL;
	}
	/* Ulozenie hodnot do struktury */
	strcpy(newItem->key, key);
	newItem->data = data;
	newItem->next = NULL;
	return newItem;
}
void hitem_free(struct htab_listitem *item)
{
	if (item==NULL) return;
	if (item->key) free(item->key);
	item->data=0;
	free(item);
}
