/** @file fold.c
 * @brief Upravuje zadany text podla zadanych parametrov
 * Riesenie IJC-DU2, priklad a)
 * Přeloženo: gcc 4.8.1
 * @author David Mikus, FIT
 * @date 26.4.2014
 */
 
#include "fold.h"

int main(int argc,char *argv[])
{
	optionsStruct options = {.file=stdin, .width=80, .c=false};	
	int value;
	char *endPtr;
	char filename[FILENAME_MAX_LENGTH];
	bool fileFound=false;
	for (int i=1;i<argc;i++)
	{
		if (!strcmp(argv[i],"-w"))
		{
			if (++i>=argc)
			{
				 fprintf(stderr,"Prepinac 'w' vyzaduje parameter\n");
				 return EXIT_FAILURE;
			}
			value=strtol(argv[i],&endPtr,10);
			if (*endPtr!=0)
			{
				fprintf(stderr,"Prepinac 'w' vyzaduje ciselny parameter\n");
				return EXIT_FAILURE;
			}
			options.width=value;					 
		}
		else if (!strcmp(argv[i],"-c"))
		{
			options.c=true;
		}
		/* Akykolvek iny argument je predpokladany ako meno suboru */
		else
		{
			if (strlen(argv[i])>=FILENAME_MAX_LENGTH)
			{
				fprintf(stderr,"Meno suboru nesmie byt dlhsie ako 255 znakov\n");
				return EXIT_FAILURE;
			}
			if(fileFound)
			{
				 fprintf(stderr,"Je mozne spracovat len 1 subor!\n");
				 return EXIT_FAILURE;
			}
			strcpy(filename,argv[i]);
			fileFound=true;
			
		}
	}
	if (fileFound) options.file = fopen(filename, "r");
	if (options.file == NULL)
	{
		fprintf(stderr,"Zadany subor sa nepodarilo otvorit\n");
		return EXIT_FAILURE;
	}
	vypis(options);
	if (options.file!= NULL && options.file != stdin) fclose(options.file);
	return EXIT_SUCCESS;
}
void vypis(optionsStruct options)
{
	/* Treba miesto aj pre ukoncovaciu nulu */
	char word[WORD_MAX_LENGTH+1]={'\0'};
	int returnValue;
	int currentPosition=0;
	bool NewLine=true;
	while((returnValue=fgetw(word,WORD_MAX_LENGTH,options.file))!=EOF)
	{	
		/* Prvy znak bol newline */
		if (returnValue==0)
		{
			currentPosition=0;
			putchar('\n');
			if (!NewLine) putchar('\n');
			NewLine=true;
			continue;
		}
		currentPosition+=returnValue;
		if (options.c && returnValue>options.width) word[options.width]=0;
		/* Treba brat aj medzeru do uvahy */
		if (!NewLine) currentPosition++;
		if (currentPosition>options.width)
		{
			currentPosition=returnValue;
			if (!NewLine) putchar('\n');
			NewLine=true;
		}
		/* Ak neni slovo nazaciatku riadku, tak vytlaci pred nim aj medzeru */
		if (!NewLine) putchar(' ');
		printf("%s",word);
		NewLine=false;
		
	}
	/* Textovy subor by mal mat na konci suboru newline */
	if (!NewLine) putchar('\n');
	
}
