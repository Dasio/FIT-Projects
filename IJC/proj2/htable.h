/** @file htable.h
 * @brief Rozhranie pre hashovaciu tabulku
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */
#ifndef HTAB_H
#define HTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_function.h"

typedef struct
{
	unsigned long size;
	struct htab_listitem *data[];
}htab_t;
struct htab_listitem
{
	char *key;
	unsigned data;
	struct htab_listitem *next;
};
htab_t *htab_init(unsigned long size);
struct htab_listitem * htab_lookup(htab_t *t, const char *key);
void htab_foreach(htab_t *t,void (*fp)(const char*,unsigned));
void htab_remove(htab_t *t,const char *key);
void htab_clear(htab_t *t);
void htab_free(htab_t *t);
void htab_statistics(htab_t *t);
/* Custom */
/**
 * @brief Inicializuje prvok
 * @param key, nazov kluca
 * @param data, prvotna hodnota
 * @return Ukazatel na inicializovany prvok
 */
struct htab_listitem *hitem_init(const char *key,unsigned data);
/**
 * @brief Uvolni prvok
 * @param item, prvok ktory sa uvolni
 */
void hitem_free(struct htab_listitem *item);

#endif /* !HTAB_H */
