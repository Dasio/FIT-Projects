/** @file htab_statistics.c
 * @brief Prejde cely zoznam a vypise nad nim statistiku
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "htable.h"

void htab_statistics(htab_t *t)
{
	if (t==NULL) return;
	int *count = malloc(sizeof(int)*t->size);
	if (count==NULL)
	{
		fprintf(stderr,"Nepodarilo sa alokovat miesto pre statistiku");
		return;
	}
	int counter=0;
	struct htab_listitem *item = NULL;
	/* Spocitanie kazdeho zaznamu */
	for (unsigned long i = 0; i<t->size; i++)
	{
		counter=0;
		for(item=t->data[i];item;item=item->next)
		{
			counter++;
		}
		count[i]=counter;
	}
	int sum=0;
	int min=count[0];
	int max=0;	
	/* Zistenie min,max a spocitanie sumy */
	for (unsigned long i =0;i<t->size; i++)
	{
		sum+=count[i];
		if(count[i]>max) max=count[i];
		else if (count[i]<min) min=count[i];
	}
	/* Zistenie priemeru */
	float avg = ((float)sum/t->size);
	printf("###### Statistika #######\n");
	printf("Min = %d\t\t\nMax = %d\t\t\nAvg = %.2f\t\t\n",min,max,avg);
	printf("#########################\n");
	free(count);
}
