/** @file eratosthenes.c
* @brief Modul pre Eratostenovo sito
* Riesenie IJC-DU1, priklad a)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 4.3.2014
*/
#include "eratosthenes.h"
#include "bit-array.h"
#include <math.h>

void Eratosthenes(BitArray_t pole)
{
	/* 0 a 1 nie su prvocisla */
	SetBit(pole,0,1);
	SetBit(pole,1,1);
	unsigned long n=2;
	/* Staci prehladavat po odmocninu zo zadaneho cisla */
	unsigned long max = sqrt(BitArraySize(pole));
	for(unsigned long i=2;i<=max;i++)
	{
		if (GetBit(pole,i)==0)
		{
			n=2;
			/* Vsetky nasobky prvocisla */		
			while(i*n<BitArraySize(pole))
			{
				SetBit(pole,i*n,1);
				n++;
			}		
		}
	}
}
