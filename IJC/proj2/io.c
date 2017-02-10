/** @file io.c
 * @brief Nacita slovo nasledujuce slovo, zo zadaneho suboru
 * Riesenie IJC-DU2, priklad b)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */

#include "io.h"

int fgetw(char *s, int max, FILE *f)
{
	int c,i;
    /* Static - aby error vytlacil len raz, aj ked sa funkcia bude volat viackrat */
    static bool errorPrinted=false;
	if ((c=fgetc(f))=='\n') return 0;
	/* V pripade ze to nebol new line feed, tak sa znak vrati */
	else ungetc(c,f);
	/* Preskocenie pripadnych bielych znakov */
	while ((c=fgetc(f))!=EOF && isspace(c));
	if (c==EOF) return EOF;
	/* Posledny precitany znak v predchadzajucom while je platny */
	s[0]=c;
	for(i=1;(c=fgetc(f))!=EOF && !isspace(c);i++)
	{
		/* Ak slovo je dlhsie ako zadane maximum */
		if (i>=max)
		{
			/* Nutne pridat ukoncovaciu nulu */
			s[i]=0; 
			/* Nutne docitat dane slovo */
			while ((c=fgetc(f))!=EOF && !isspace(c)); 
            if (!errorPrinted)
            {
			    fprintf(stderr,"Nacitane dlhe slovo, orezavam\n");
                errorPrinted=true;
            }
			return i;
		}
		s[i]=c;
	}
	s[i]=0;
	return i;
}
