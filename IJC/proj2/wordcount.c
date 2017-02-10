/** @file wordcount.c
 * @brief Spocita slova zo stdin
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "io.h"
#include "htable.h"

#define WORD_MAX_LENGTH 127 
/* Na zaklade hladania bolo zistene ze velkost tabulky by malo byt prvocislo
 * http://planetmath.org/goodhashtableprimes
 * Statistika: testovane na /usr/share/dict/words
 * 12289:	Min = 0
			Max = 22
			Avg = 8.07
 * 24593:	Min = 0
			Max = 13
			Avg = 4.03
 * 49157:	Min = 0
			Max = 11
			Avg = 2.02
 * Priemer sa 2x snizuje,min zostava 0, max u 24593 bolo skoro 2x mensie ako u 12289,
	u 49157 to bolo len o 2menej. Tak sa rozhodlo pre 24593 */
#define HASH_TAB_SIZE 24593
void print_item(const char* name,unsigned data)
{
	printf("%s\t%u\n",name,data);
}
int main()
{
	htab_t *hashTable = htab_init(HASH_TAB_SIZE);
	struct htab_listitem *item;
	if (hashTable==NULL)
	{
		fprintf(stderr,"Nepodarilo sa alokovat miesto pre hashtable\n");
		return EXIT_FAILURE;
	}
	int returnValue=0;
	char word[WORD_MAX_LENGTH+1]={'\0'};
	while((returnValue=fgetw(word,WORD_MAX_LENGTH,stdin))!=EOF)
	{
		/* Najdeny newline, preskocit */
		if (returnValue==0) continue;
		if ((item=htab_lookup(hashTable,word))==NULL)
		{
			htab_free(hashTable);
			return EXIT_FAILURE;
		}
		(item->data)++;
	}
	htab_foreach(hashTable,print_item);
	htab_free(hashTable);
	return EXIT_SUCCESS;
	
}
