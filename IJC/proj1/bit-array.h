/** @file bit-array.h
* @brief Implementacia rozhrania pre bitove polia
* Riesenie IJC-DU1, priklad a)
* Přeloženo: gcc 4.8.1
* @author David Mikus, FIT
* @date 4.3.2014
*/
#include <limits.h>
#include "error.h"

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H
typedef unsigned long BitArray_t[];

/* Pocet bitov v unsigned long */
#define UL_BITS (CHAR_BIT*sizeof(unsigned long))

/* Kolko prvkov pola unsigned long je potreba
 * Ak velkost neni nasobkom poctu bitov UL, tak sa musi zobrat o jeden prvok naviac
 */
#define UL_ELEMENTS(size) (((size)%UL_BITS==0)?((size)/UL_BITS):((size)/UL_BITS+1))

/* Alokuje potrebnu pamat pre danu velkost
 * Iniciliazuje vsetky prvky na nulu
 * Na prvy ([0]) prvok sa ulozi velkost bitoveho pola
 */
#define BitArray(jmeno_pole,velikost) unsigned long jmeno_pole[UL_ELEMENTS(velikost)+1] = {(unsigned long)(velikost)}

/* Zisti pocet bitov typu daneho bitoveho pola */
#define TYPE_BITS(jmeno_pole) (CHAR_BIT*sizeof(jmeno_pole[0])) 

/* Zisti index pola na ktorom sa nachadza index 'i' */
#define TYPE_ELEMENTS(p,i) (i/TYPE_BITS(p))
	
#ifndef USE_INLINE

/* Vypise prvy prvok pola, kde je uchovana velkost bitoveho pola */
#define BitArraySize(jmeno_pole) (jmeno_pole[0])

/*  Vytvori pomocnu masku posunutim 1 do lava na zaklade zadaneho indexu,
 *	a spravi bitovu operaciu AND nad prvkom pola, vysledok porovna a vrati 0 alebo 1 
 */

#define DU1__GET_BIT(p,i) (((p[TYPE_ELEMENTS(p,i)+1]&(1UL<<(i%TYPE_BITS(p))))) > 0 ? 1 : 0)

/* b==0: Vytvori pomocnu masku posunutim 1 do lava na zaklade zadaneho indexu,
 * 		 zneguje a nasledne spravi bitovu operaciu AND nad prvkom pola
 * 		 a hodnotu ulozi
 * b!=0: Vytvori pomocnu masku posnutim 1 do lava na zaklade zadaneho indexu
 * 		 a nasledne spravi bitovu operaciu OR nad prvkom pola a hodnotu ulozi
 * */
#define DU1__SET_BIT(p,i,b) (b==0)?(p[TYPE_ELEMENTS(p,i)+1] &= ~(1UL<<(i%TYPE_BITS(p)))):((p[TYPE_ELEMENTS(p,i)+1]) |= (1UL<<(i%TYPE_BITS(p))))

/* Kontroluje meze pola, ak je index mimo mezi zavola sa funkcia FatalError,1
 * FatalError je void, s pouzitim operatoru ',' sa void zahodi a vrati 1(chyba)
 * Inak zavola DU1__GET_BIT
 */
#define GetBit(jmeno_pole,index) (((unsigned long)(index)>=jmeno_pole[0])\
		?(FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]-1)),1\
		:(DU1__GET_BIT(jmeno_pole,index)))

/* Kontroluje meze pola, ak je index mimo mezi zavola sa funkcia FatalError,1
 * FatalError je void, s pouzitim operatoru ',' sa void zahodi a vrati 1(chyba)
 * Inak zavola DU1__SET_BIT
 */
#define SetBit(jmeno_pole,index,vyraz) (((unsigned long)(index)>=jmeno_pole[0])\
		?(FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]-1)),1\
		:(DU1__SET_BIT(jmeno_pole,index,vyraz)))
		
#else /* USE_INLINE definovane */

/* Vypise prvy prvok pola, kde je uchovana velkost bitoveho pola */
inline unsigned long BitArraySize(BitArray_t jmeno_pole)
{
	return jmeno_pole[0];
}
inline int GetBit(BitArray_t jmeno_pole,unsigned long index)
{
	if (index>=BitArraySize(jmeno_pole)) FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]-1);
	return (jmeno_pole[TYPE_ELEMENTS(jmeno_pole,index)+1]&(1UL<<(index%TYPE_BITS(jmeno_pole))))>0?1:0;
}

inline void SetBit(BitArray_t jmeno_pole,unsigned long index,int vyraz)
{
	if (index>=BitArraySize(jmeno_pole)) FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]-1);
	if (vyraz==0) jmeno_pole[TYPE_ELEMENTS(jmeno_pole,index)+1] &= ~(1UL<<(index%TYPE_BITS(jmeno_pole)));
		else jmeno_pole[TYPE_ELEMENTS(jmeno_pole,index)+1] |= (1UL<<(index%TYPE_BITS(jmeno_pole)));
}

#endif /* USE_INLINE */		
#endif /* !BIT_ARRAY_H */
