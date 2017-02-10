/** @file steg-encode.c
* @brief Zakoduje spravu v ppm
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 4.3.2014
*/
#include <string.h>
#include <stdbool.h>
#include "bit-array.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"
#include <ctype.h>
#define MAX_LIMIT 1000*1000*3
void showHelp()
{
	printf("Usage: ./steg-encode.o file1.ppm encodedfile.ppm \"text to encode\"\n");  
}
void EncodeMessage(struct ppm *ppmFile,char* text)
{
	BitArray(primes,MAX_LIMIT);
	Eratosthenes(primes);
	char encode[2],znak[2];
	bool end=false;
	encode[0] = CHAR_BIT;
	znak[0] = CHAR_BIT;
	short int j=0;
	unsigned n=0;
	znak[1] = text[n];
	for (unsigned long i=2;i<MAX_LIMIT;i++)
	{
		if(i>3*ppmFile->xsize*ppmFile->ysize) break;
		if (GetBit(primes,i)==0)
		{
			/* Pomocna premenna kde sa nastavi prislusny bit */
			encode[1]=ppmFile->data[i];
			//SetBit(encode,0,GetBit(znak,j));
			/* SetBit ani GetBit nepouzity lebo ocakava long unsigned int* */
			DU1__SET_BIT(znak,j,DU1__GET_BIT(znak,j));
			/* Po zakodovani sa ulozi spat do struktury */
			ppmFile->data[i]=encode[1];
			j++;
			if (j==CHAR_BIT)
			{
				/* Nulovy znak zapisany, cyklus sa moze ukocnit */
				if (end) break;
				j=0;
				znak[1]=text[++n];
				if (znak[1]==0) end=true;
			}
		}
	}
}
int main(int argc,char *argv[])
{
	if (argc==2 && (strcmp(argv[1],"-help")==0))
	{
		showHelp();
		return 0;
	}
	if (argc!=4) 
	{		
		Warning("Zle zadane argumenty\n");
		showHelp();
		return 1;
	}
	struct ppm *ppmFile=ppm_read(argv[1]);
	if (ppmFile==NULL)
	{
		FatalError("Chyba citania zo suboru");
		return 1;
	}
	EncodeMessage(ppmFile,argv[3]);
	if (ppm_write(ppmFile,argv[2])!=0)
	{
		free(ppmFile);
		Warning("Zapis do suboru sa nepodaril");
		return 1;
	}
	free(ppmFile);
	return 0;
}
