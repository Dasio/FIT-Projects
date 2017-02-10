#include "htable.h"

htab_t *htab_init(unsigned long size)
{
	if (size<1) return NULL;
	htab_t *new_htab;
	/* Alokujeme miesto pre strukturu a flexible member array */
	new_htab=malloc(sizeof(htab_t)+sizeof(struct htab_listitem *)*size);
	if (new_htab==NULL) return NULL;
	/* Nastavime vsetky ukazatele na NULL */
	for(unsigned i=0;i<size;i++)
		new_htab->data[i]=NULL;
	new_htab->size=size;
	return new_htab;
}

