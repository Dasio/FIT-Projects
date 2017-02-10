/** @file ppm.c
* @brief Modul pre citanie a zapisovanie ppm formatu.
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 3.3.2014
*/
#include "ppm.h"

struct ppm * ppm_read(const char *filename)
{
	/* Otvorenie suboru z ktoreho sa budu citat data */
	FILE *fileRead = fopen(filename,"rb");
	if (fileRead==NULL)
	{
		Warning("Nepodarilo sa otvorit subor");
		return NULL;
	}
	/* Precitanie rozmerov a pocet farieb */
	unsigned  x,y,colors;
	int read = fscanf(fileRead,"P6 %u %u %u",&x,&y,&colors);
	if (read!=3 || colors!=255) 
	{
		Warning("Nepodarilo sa nacitat korektnu hlavicku");
		fclose(fileRead);
		return NULL;
	}
	/* Predpoklada sa whitespace char */
	char checkWhitespace;	
	if (fscanf(fileRead,"%c",&checkWhitespace)!=1)
	{
		Warning("Nepodarilo sa nacitat biely znak");
		fclose(fileRead);
		return NULL;
	} 
	if (!isspace(checkWhitespace))
	{
		Warning("Hlavicka neni oddelena bielym znakom od binarnych dat");
		fclose(fileRead);
		return NULL;
	}
	/* Alokovanie pamate pre strukturu vratane "flexible array member", sizeof(char)=1 */
	struct ppm * ppmStruct = malloc(sizeof(struct ppm)+3*x*y);
	if (ppmStruct == NULL)
	{
		Warning("Nepodarilo sa alokovat pamat pre strukturu");
		fclose(fileRead);
		return NULL;
	}
	/* Nacitanie binarnych dat do data[], 3*x*y prvkov, kazdy prvok ma 1 byte(char) */
	if (fread(ppmStruct->data,1,3*x*y,fileRead)!=3*x*y)
	{
		Warning("Nepodarilo sa nacitat binarne data");
		free(ppmStruct);
		fclose(fileRead);
		return NULL;
	}
	ppmStruct->xsize=x;
	ppmStruct->ysize=y;
	fclose(fileRead);
	return ppmStruct;
}
int ppm_write(struct ppm *p, const char *filename)
{
	/* Otvorenie suboru do ktoreho sa budu zapisovat data */
	FILE *fileWrite = fopen(filename,"wb");
	if (fileWrite==NULL)
	{
		Warning("Nepodarilo sa otvorit subor pre zapis");
		return -1;
	}
	/* Zapisanie hlavicky */
	fprintf(fileWrite,"P6\n%u %u\n255\n",p->xsize,p->ysize);
	/* Zapisanie binarnych dat */
	if (fwrite(p->data,1,3*p->xsize*p->ysize,fileWrite)!=3*p->xsize*p->ysize)
	{
		Warning("Nepodarilo sa zapisat data");
		fclose(fileWrite);
		return -1;
	}
	fclose(fileWrite);
	return 0;
}
