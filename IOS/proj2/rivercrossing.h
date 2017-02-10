/** @file rivercrossing.h
 * @brief Rozhranie pre rivercrossing.c
 * @author David Mikus, FIT (xmikus15@stud.fit.vutbr.cz)
 * @date 29.4.2014
 */
#ifndef RC_H
#define RC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
/** Vracajuce hodnoty funkcii */
enum Estatus
{
	OK = 0x00, /**< Vsetko prebehlo vporiadku */
	ERR = 0x01, /**< Vyskytla sa chyba, neocakavane parametre */
	ERR_SYS = 0x02, /**< Nastala chyba pri systemovom volani */
	ERR_S = 0x04, /**< Nastala chyba pri praci so semaforom */
	ERR_SHM = 0x08 /**< Nastala chyba pri praci so zdielanou pametou */
};
/** Identifikacia semaforov */
enum Esemaphore
{
	EXCLUSIVE, /**< Vylucny pristup k zapisu */
	HACKER, /**< Vstup hackera na lod */
	SERF, /**< Vstup serfa na lod */
	MOLO, /**< Vstup osoby na molo */
	BOARDING, /**< Kym sa vsetci nalodia */
	LANDING, /**< Kym vsetci na lodi vypisu kategoriu */
	FINISH, /**< Kym vsetky procesy sa vylodia */
	CAPTAIN, /**< Plavba lode, cakanie na uspaneho kapitana */
	MAX_SEM /**< Pocet semaforov */
};
/** Identifikacia zdielanej pamete */
enum Esharedmemory
{
	COUNTER, /**< Poradove cislo akcie */
	HACKERS_COUNT, /**< Pocet hackerov na mole */
	SERFS_COUNT, /**< Pocet serfov na mole */
	WAITING_MEMBERS, /**< Pomocna premenna pre zistenie posledneho procesu na lodi */
	WAITING_TO_FINISH, /**< Pomocna premenna pre zistenie posledneho procesu */
	MAX_MEM /**< Pocet zdielanych premennych */
};
/** Struktura pre obsah argumentov */
typedef struct
{
	int p; /**< Kolko hackerov/serfov sa ma vygenerovat */
	int h; /**< Za aky maximalny cas(v ms) sa vygeneruje hacker */
	int s; /**< Za aky maximalny cas(v ms) sa vygeneruje serf */
	int r; /**< ako maximalne dlho moze trvat plavba(v ms) */
	int statusCode; /**< Navratovy kod funkcie pre spracovanie argumentov */
} TParams;

/**
 * @brief Generator osob(hacker/serf)
 * 
 * @param p Kolko osob sa ma vytvorit
 * @param delay Maximalny cas(v ms) za ktory vytvori dalsiu osobu
 * @param isHacker Urcuje si ma generovat hackera alebo serfa
 * @param sailTime Ako dlho moze maximalne trvat plavba(v ms)
 */
void generate(int p,int delay,bool isHacker,int sailTime);

/**
 * @brief Proces(hacker/serf)
 * 
 * @param id ID osoby, stara sa on generator osob
 * @param isHacker Urcuje ci je osoba hacker alebo serf
 * @param sailTime Ako dlho bude plavba trvat
 * @param maxMembers Kolko osob sa musi vylodit
 */ 
void member(int id,bool isHacker,int sailTime,int maxMembers);

/**
 * @brief Spracuje argument z prikazoveho riadku
 * 
 * @param argc Pocet argumentov
 * @param **argv Ukazatel na pole argumentov
 * @param *params Kam sa spracovane argumenty maju ulozit
 */
void processArguments(int argc,char **argv,TParams *params);

/**
 * @brief Alokuje a otvori vsetko potrebne pre semafory a zdielanu pamet
 * @return OK, ak sa vsetko potrebne podarilo alokovat
 *	ERR_S, ak sa nepodaril alokovat semafor
 * 	ERR_SHM, ak sa nepodarilo alokovat zdielanu pamet
 */
int init();

/**
 * @brief Odstrani semafori a zdielanu pamet
 * @return OK, ak sa vsetko podarilo uvolnit
 * Flag ERR_S, nepodarilo sa uvolnit aspon jeden semafor
 * Flag ERR_SHM, nepodarilo sa uvolnit aspon jednu zdielanut pamet
 */
int cleanup();
/**
 * @brief Hl.proces posle signal ostatnym procesom, zavola cleanup(),uzavrie subor pre vypis a proces ktory ho volal konci s navratovym kodom ERR_SYS(2)
 */
void cleanAndExit();
/**
 * @brief Vola sa ked nastane chyba v childovi, robi to iste ako cleanAndExit(), len nerobi cleanup()
 */
void childExit();
#endif
