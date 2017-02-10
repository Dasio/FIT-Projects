/*
 * Subor: proj1.c
 * Datum: 2013/11/07
 * Autor: David Mikus, xmikus15@stud.fit.vutbr.cz
 * Projekt: 1. projekt IZP na FIT VUT
 * Popis: Program nacita vstup a spocita slova podla zadanych argumentov
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
// Maximalna dlzka slova pre debug mode
#define DEBUG_MAXLEN 80
// Magicke hodnoty pre specialne znaky
#define CHAR_ALL '.'
#define CHAR_NUMBER ':'
#define CHAR_UPCASE '^'
typedef struct
{
    char symbol;
    unsigned int index;
    bool debug;
}  TParams;
void showError()
{
    fprintf(stderr,"Chyba!\n"
    "Zle zadane parametre! Pre napovedu pouzite prepinac --help\n"
    "Program sa ukonci!\n");
}
void showHelp()
{
    printf("\nProgram sa spusta v podobe ./proj1 X [N] [-d]\n"
    "\nArgumenty programu:\n"
    "\nX reprezentuje hledany znak v pocitanych slovech. Specialni znak : navic symbolizuje libovolne cislo 0-9, znak ^ symbolizuje libovolne velke pismeno A-Z a znak . symbolizuje libovolny znak.\n"
    "\nVolitelny argument N predstavuje cislo vyjadrujici pozici hledaneho znaku v pocitanych slovech. Platne pocitane slovo musi mit hledany znak prave na dane pozici. Cislo N je kladne - prvni znak ve slove ma pozici 1.\n"
    "\n- N moze byt aj cislo 0, vtedy hlada na akekolvej pozicii, ak nieje argument N specifikovany automaticky sa pocita s 0\n"
    "\n- N nemoze nadobudat vacsie hodnoty ako INT_MAX, ak bude zadane vacsie cislo tak sa implicitne nastavi 0\n"
    "\nVolitelny argument -d zapina tzv. ladici (debug) rezim programu. V ladicim rezimu program na vystup prubezne vypisuje kazde nactene slovo, jedno slovo na radek. Maximalna dlzka slova je 80znakov.\n\n");
}
bool checkDigit(char charToCheck)
{
    return (charToCheck>='0' && charToCheck<='9');
}
bool checkUpperCase(char charToCheck)
{
    return (charToCheck>='A' && charToCheck<='Z');
}
bool checkLowerCase(char charToCheck)
{
    return (charToCheck>='a' && charToCheck<='z');
}
bool checkCharInput(char charToCheck)
{
    return (checkDigit(charToCheck) || checkUpperCase(charToCheck) || checkLowerCase(charToCheck) || charToCheck=='-' || charToCheck=='_');
}
// Na zaklade zadanemu znaku zo stdin a symbolu z argumentu zisti ci dany znak vyhovuje symbolu
bool checkChar(char charToCheck,char symbolToCheck)
{
    if (symbolToCheck==CHAR_NUMBER) return(checkDigit(charToCheck));
        else if (symbolToCheck==CHAR_UPCASE) return(checkUpperCase(charToCheck));
        else if (symbolToCheck==CHAR_ALL || symbolToCheck==charToCheck) return true;
        else return false;
}
// Prevedie cislo v chare na int
unsigned int charToInt(char *charNumber)
{
    unsigned long long int number = 0;
    for (unsigned int i = 0; charNumber[i] != '\0'; i++)
    {
        if (!checkDigit(charNumber[i])) return 0;
        number = number*10 + charNumber[i] - '0';
        // Ak nastane overflow
        if (number>INT_MAX) return 0;
    }
    return (unsigned int)number;
}
/*
 * Nacita vstup a spocita slova
 * Ako parameter je mu predana struktura ktora obsahuje ktory znak hladat,na ktorej pozicii a ci zapnut ladiaci rezim
 */
int readAndCount(TParams params)
{
 short int c=0;
 bool wordFound=false; // Kontroluje ci uz bolo dane slovo zapocitane
 bool newWord=true;    // Kontrola pre debug mod ci ma vytlacit \n
 unsigned int position=0;
 unsigned int wordsCount=0;
 //Nacitavane vstupu dokym nenarazi na EOF
 while ((c = getchar())!=EOF)
 {
     // Ak je znak oddelovac slov
     if(!checkCharInput(c))
     {
        if (params.debug && !newWord) putchar('\n');
        wordFound=false;
        newWord=true;
        position=0;
        continue;
     }
     if (params.debug && position<DEBUG_MAXLEN) putchar(c);
     newWord=false;
     position++;
     // Ak dany znak je ten ktory hladame
     if (!wordFound && checkChar(c,params.symbol) && (!params.index || params.index==position))
     {
          wordFound=true;
          wordsCount++;
     }

 }

 return wordsCount;
}

int main(int argc,char *argv[])
{
   TParams params;
   params.index=0;
   params.debug=false;
   unsigned int index=0;
   if (argc < 2 || argc > 4)
   {
        showError();
        return EXIT_FAILURE;
   }
    // Spracovanie parametrov
    if (strcmp(argv[1],"--help")==0)
    {
        showHelp();
        return EXIT_SUCCESS;
    }
    if (strlen(argv[1])>1)
    {
        showError();
        return EXIT_FAILURE;
    }
    // Nacitanie znaku pre ,hladanie
    params.symbol = argv[1][0];
    // Prejdene volitelnych argumentov
    for (int i=2;i<argc;i++)
    {
        index=charToInt(argv[i]);
        if (index!=0) params.index=index;
        if (strcmp(argv[i],"-d") == 0) params.debug=true;
    }
    // Nacitanie vstupu a vypis
    printf("%d\n",readAndCount(params));

return EXIT_SUCCESS;
}
