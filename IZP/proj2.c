/*
 * Subor: proj2.c
 * Datum: 2013/11/24
 * Autor: David Mikus, xmikus15@stud.fit.vutbr.cz
 * Projekt: 2. projekt IZP na FIT VUT
 * Popis: Program na vypocet sqrt,arc sin a uhlov v trojuholniku
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define optimalizationNumber 0.7072
#define error {showError();return EXIT_FAILURE;}
const double inf = 1.0/0.0;
const double NAN=0.0/0.0;
const double pi2=1.570796326794896619231321691639751442098584699687;
const double eps=1e-14;

 typedef struct{
     double x,y;
 } TCoords;

 typedef struct{
    double alfa,beta,gama;
} TAngle;

 typedef struct {
     TCoords a,b,c;
 } TTriangle;

void showError()
{
    fprintf(stderr,"Chyba!\n"
    "Zle zadane parametre! Pre napovedu pouzite prepinac --help\n"
    "Program sa ukonci!\n");
}
void showHelp()
{
    printf("Argumenty programu:\n"
            "--help  zpusobi, ze program vytiskne napovedu pouzivani programu a skonci.\n"
            "--sqrt X vypocita a vypise druhou odmocninu z cisla X.\n"
            "--asin X vypocita a vypise arcus sinus z cisla X.\n"
            "--triangle AX AY BX BY CX CY vypocita a vypise tri uhly troujuhelniku, ktery je dan vrcholy A=AX,AY, B=BX,BY a C=CX,CY.\n");
}
double my_fabs(double x)
{
    return (x<0.0) ? -x : x;
}

double my_sqrt(double x)
{
 if (x==inf) return inf;
 if (x==0.0) return 0;
 if (x<0.0) return NAN;
 double sqrt=x;
 double diff=0;
 do
 {
    diff=sqrt;
    //Newtonova metoda
    sqrt=((x/sqrt)+sqrt)/2;
    diff-=sqrt;
 } while (my_fabs(diff)>eps*sqrt);
return sqrt;
}

double my_asin(double x)
{
    short int sign=1;
    if (x<0.0) sign=-1;
    x=my_fabs(x);
    if (x>1.0) return NAN;
    double x2=x*x;
    // Pre rychlejsie konvergovanie
    if (x>optimalizationNumber) return sign*(pi2-my_asin(my_sqrt(1-x2)));
    double asin=x;
    int i=1;
    do
    {
        //Taylorova rada
        x*=((x2)*(2*i-1)*(2*i-1))/((2*i)*(2*i+1));
        asin+=x;
        i++;

    } while (my_fabs(x)>eps*asin);
    return sign*asin;
}
double my_acos(double x)
{
    return pi2-my_asin(x);
}
double getVectorLength(TCoords a,TCoords b)
{
    double x=b.x-a.x;
    double y=b.y-a.y;
    return my_sqrt(x*x+y*y);
}
/*bool isTriangle(double a,double b,double c)
{
    // Trojuholnikova nerovnost
    return (a+b>c && a+c>b && b+c>a);
}*/
bool isTriangle(TTriangle triangle)
{
    // Kolinearnost
   return !(((triangle.c.x-triangle.a.x)/(triangle.b.x-triangle.a.x) == (triangle.c.y-triangle.a.y)/(triangle.b.y-triangle.a.y))
            || ((triangle.c.x-triangle.a.x) == 0 && (triangle.b.x-triangle.a.x) == 0)
            || ((triangle.c.y-triangle.a.y) == 0 && (triangle.b.y-triangle.a.y) == 0));

}
TAngle calcTriangleAngles(TTriangle triangle)
{
    TAngle angle =
    {
        .alfa=NAN,
        .beta=NAN,
        .gama=NAN
    };
    if(isTriangle(triangle))
    {
    double a=getVectorLength(triangle.b,triangle.c);
    double b=getVectorLength(triangle.a,triangle.c);
    double c=getVectorLength(triangle.a,triangle.b);
    //if(isTriangle(a,b,c))
    angle.alfa=my_acos(-(a*a-b*b-c*c)/(2*b*c));
    angle.beta=my_acos(-(b*b-a*a-c*c)/(2*a*c));
    angle.gama=my_acos(-(c*c-a*a-b*b)/(2*a*b));
    }
    return angle;
}
int main(int argc,char *argv[])
{
    if (argc<2 || argc>8) error;
    double number = 0;
    char *err;
    if (strcmp(argv[1],"--help")==0)
    {
        showHelp();
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[1],"--sqrt")==0)
    {
        if (argc!=3) error;
        number = strtod(argv[2],&err);
        if (*err != 0) error;
        printf("%.10e\n",my_sqrt(number));
    }
    else if (strcmp(argv[1],"--asin")==0)
    {
        if (argc!=3) error;
        number = strtod(argv[2],&err);
        if (*err != 0) error;
        printf("%.10e\n",my_asin(number));
    }
    else if (strcmp(argv[1],"--triangle")==0)
    {
        if (argc!=8) error;
        TTriangle triangle;
        TAngle angle;
        for (int i = 0;i<6;i++)
        {
          number = strtod(argv[i+2],&err);
          if (*err != 0) error;
          // Uklada postupne hodnoty do struktury a.x,a.y,b.x ...
          ((double*)&triangle)[i] = number;
        }
        angle = calcTriangleAngles(triangle);
        printf("%.10e\n%.10e\n%.10e\n",angle.alfa,angle.beta,angle.gama);
    } else error;
    return EXIT_SUCCESS;
}
