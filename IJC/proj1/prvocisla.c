/** @file prvocisla.c
* @brief Vypise poslednych 10 prvocisiel
* Riesenie IJC-DU1, priklad a)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 4.3.2014
*/
#include <stdio.h>
#include "bit-array.h"
#include "error.h"
#include "eratosthenes.h"
#define NUMBERS_TO_PRINT 10
#define LIMIT 101000000
int main()
{
	/* +1, lebo inak by nebralo posledne cislo v LIMIT 
	 * vytvorilo by sa miesto len 0..LIMIT-1 */
	BitArray(primes,LIMIT+1);
	Eratosthenes(primes);
	int count=0;
	int i;
	/* Zistenie indexu prvocisla 10. od konca */
	for(i=LIMIT-1;count<NUMBERS_TO_PRINT;i--)
	{
		if (GetBit(primes,i)==0) count++;
	}
	/* Vypis poslednych 10 prvocisiel */
	for(int j=i;j<=LIMIT;j++)
	{
		if (GetBit(primes,j)==0) printf("%d\n",j);
	}	
	return 0;
}

