/** @file steg-decode.c
* @brief Rozlusti spravu v ppm
* Riesenie IJC-DU1, priklad b)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 4.3.2014
*/
#include <stdbool.h>
#include "bit-array.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"
#include <ctype.h>
#define MAX_LIMIT 1000*1000*3

void PrintMessage(struct ppm *ppmFile)
{		
	BitArray(primes,MAX_LIMIT);
	Eratosthenes(primes);
	unsigned int j=0;
	bool end=false;
	char znak[2];
	/* Get/SetBit predpokladaju na prvom prvku velkost */
	znak [0] = CHAR_BIT;
	znak [1] = 0;
	for(unsigned long i=2;i<MAX_LIMIT;i++)
	{
		if (GetBit(primes,i)==0)
		{
			/* Nastavi bit podla LSB prvociselneho bajtu */
			/* SetBit nepouzity lebo ocakava long unsigned int* */
			DU1__SET_BIT(znak,j,(ppmFile->data[i] &1));
			if (j<CHAR_BIT-1) j++;
			/* Ak sa nacitalo 8bitov(CHAR_BIT) a su nulove tak sa cyklus ukonci */
			else if(znak[1]==0)
			{
				end=true;
				break;
			}		 
			/* Ked sa nacita vsetkych 8bitov(CHAT_BIT) tak sa vytlaci znak */
			else if (j==CHAR_BIT-1)
			{
				if (!isprint(znak[1])) FatalError("Nacitany netlacitelny znak");
				printf("%c",znak[1]);
				j=0;
			}
		}
	}
	if (!end) FatalError("Sprava nebola ukoncena nulovym znakom");
	//putchar('\n');	
}

int main(int argc,char *argv[])
{
	if (argc!=2) return 1;
	struct ppm *ppmFile=ppm_read(argv[1]);
	if (ppmFile==NULL)
	{
		FatalError("Chyba citania zo suboru");
		return 1;
	}
	PrintMessage(ppmFile);
	free(ppmFile);
	return 0;
}
