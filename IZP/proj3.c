/*
 * Subor: proj3.c
 * Datum: 2013/12/13
 * Autor: David Mikus, xmikus15@stud.fit.vutbr.cz
 * Projekt: 3. projekt IZP na FIT VUT
 * Popis: Program na hladanie utvarov v bitovej mape
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct{
    int rows;
    int cols;
    char *cells;
} Bitmap;
enum tstates
{
    HELP,
    HLINE,
    VLINE,
    SQUARE,
    TEST,
    ERROR
};
enum terrors
{
    IARGUMENT,
    IALLOC
   // IBITMAP
};
typedef struct{
 int state;
 char *file;
} TParams;
void showError(int n)
{
    fprintf(stderr,"Chyba!\n");
    switch (n)
    {
        case IARGUMENT:
                fprintf(stderr,"Zle zadane parametre! Pre napovedu pouzite prepinac --help\n");
                break;
        case IALLOC:
                fprintf(stderr,"Nepodarilo sa alokovat pamat");
                break;
        /*case IBITMAP:
                fprintf(stderr,"Subor obsahuje invalidnu bitmapu\n");
                break;*/
    }
    fprintf(stderr,"Program sa ukonci!\n");

}
/*void notFound()
{
    printf("Zadany utvar nebol v danej bitovej mape najdeny.\n");
}*/
void showHelp()
{
    printf("Argumenty programu:\n"
            "\t--help vypise napovedu\n"
            "\t--test skontroluje ci zadany subor obsahuje validnu bitovu mapu\n"
            "\t--hline v danem obrazku nalezne a vytiskne pocatecni a\n\t\tkoncovou souradnici prvni nejdelsi horizontalni usecky.\n"
            "\t--vline v danem obrazku nalezne a vytiskne pocatecni a\n\t\tkoncovou souradnici prvni nejdelsi vertikalni usecky.\n"
            "\t--square v danem obrazku nalezne a vytiskne pocatecni a\n\t\tkoncovou souradnici prvniho nejvetsiho ctverce.\n");
}
char getcolor(Bitmap *bitmap, int x,int y)
{
    return bitmap->cells[y*bitmap->cols + x];
}
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    bool found=false;
    int a=0;
    int b=0;
    int n=0;
    int max=0;
    for (int i=0;i<bitmap->cols;i++)
    {
        n=0;
        found=false;
        for (int j=0;j<bitmap->rows;j++)
        {
            if (getcolor(bitmap,i,j)=='0')
            {
                n=0;
                found=false;
            }
            else
            {
                if (!found)
                    {
                        a=j;
                        b=i;
                        found=true;
                    }
                   if (n>max)
                    {
                        max=n;
                        *x1=a;
                        *y1=b;
                        *x2=j;
                        *y2=i;
                   }
                   if (n==max && *x1>a)
                   {
                        *x1=a;
                        *y1=b;
                        *x2=j;
                        *y2=i;
                   }
                n++;
            }
        }
    }
    if (max>0) return 1;
    else return 0;
}
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    bool found=false;
    int a=0;
    int b=0;
    int n=0;
    int max=0;
    for (int i=0;i<bitmap->rows;i++)
    {
        n=0;
        found=false;
        for (int j=0;j<bitmap->cols;j++)
        {
        if (getcolor(bitmap,j,i)=='0')
        {
            n=0;
            found=false;
        }
        else
            {
            if (!found)
                {
                    a=i;
                    b=j;
                    found=true;
                }
               if (n>max)
                {
                    max=n;
                    *x1=a;
                    *y1=b;
                    *x2=i;
                    *y2=j;
               }
               n++;
            }
        }
    }
    if (max>0) return 1;
    else return 1;
}
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int x,y;
    int n;
    int max=1;
    bool check;
    bool end;
    bool good=false;
    for (int j=0;j<bitmap->rows;j++)
    {
        if (max>=bitmap->rows-j) break;
        for (int i=0;i<bitmap->cols;i++)
        {
         end=false;
         if (max>=bitmap->cols-i) break;
           if (getcolor(bitmap,i,j)=='1')
            {
                check=true;
                for(n=max+1;n<=bitmap->cols-i && n<=bitmap->rows-j && !end;n++)
                {
                    good=true;
                    if(getcolor(bitmap,n+i-1,j)=='0')
                    {
                        i+=n-1;
                        end=true;
                        good=false;
                        break;
                    }
                    if (good) if(getcolor(bitmap,i,n+j-1)=='0')
                    {
                        end=true;
                        good=false;
                        break;
                    }
                    for(x=i+1;x<n+i-1 && check && good;x++)
                    {
                        if(getcolor(bitmap,x,j)=='0')
                        {
                            i+=n-1;
                            good=false;
                            end=true;
                            break;
                        }
                    }
                    for(y=j+1;y<n+j-1 && check && good;y++)
                    {
                        if (getcolor(bitmap,i,y)=='0')
                        {
                            good=false;
                            end=true;
                            break;
                        }
                    }
                    for(x=i+1;x<n+i && good;x++)
                    {
                        if(getcolor(bitmap,x,j+n-1)=='0')
                        {
                            good=false;
                            break;
                        }
                    }
                    for(y=j+1;y<n+j-1 && good;y++)
                    {
                        if (getcolor(bitmap,i+n-1,y)=='0')
                        {
                            good=false;
                            break;
                        }
                    }
                    if (good && n>max)
                    {
                        max=n;
                        *x1=j;
                        *y1=i;
                        *x2=j+max-1;
                        *y2=i+max-1;
                    }
                    check=false;
                }
            }
        }
    }
    if (max>1) return 1;
    else return 0;
}
void* initialize_bitmap(Bitmap *bitmap,int rows,int cols)
{
    bitmap->rows = rows;
    bitmap->cols = cols;
    return bitmap->cells = malloc(rows*cols*sizeof(*bitmap->cells));
}
bool load_bitmap(Bitmap *bitmap,FILE *f)
{
    int x;
    for (int i=0;i<bitmap->cols*bitmap->rows;i++)
    {
        if (fscanf(f,"%d",&x)<1) return false;
        if (x<0 || x>1) return false;
        bitmap->cells[i]=(x+'0');
    }
    if (fscanf(f,"%d",&x)==1) return false;
    return true;
}
void free_bitmap(Bitmap *bitmap)
{
    free(bitmap->cells);
}
TParams processArgs(int argc,char **argv)
{
    TParams params;
    if (argc<2 || argc>4)
        {
            params.state=ERROR;
            return params;
        }
    if (strcmp(argv[1],"--help")==0) params.state=HELP;
    else if (strcmp(argv[1],"--hline")==0) params.state=HLINE;
    else if (strcmp(argv[1],"--vline")==0) params.state=VLINE;
    else if (strcmp(argv[1],"--square")==0) params.state=SQUARE;
    else if (strcmp(argv[1],"--test")==0) params.state=TEST;
    else params.state=ERROR;
    if (argc==3){
    params.file = malloc(strlen(argv[2])+1);
    strcpy(params.file,argv[2]);
    }
    return params;
}
int main(int argc,char **argv)
{
    TParams params = processArgs(argc,argv);
    if (params.state==ERROR)
    {
      showError(IARGUMENT);
      return EXIT_FAILURE;
    }
    if (params.state==HELP)
    {
      showHelp();
      return EXIT_SUCCESS;
    }
    int x1=0,x2=0,y1=0,y2=0;
    int rows,cols;
    FILE *f;
    if ((f = fopen(params.file,"r"))==NULL)
    {
      perror("Error");
      return EXIT_FAILURE;
    }
    if (fscanf(f,"%d %d",&rows,&cols)<2)
    {
        printf("Invalid\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    Bitmap bitmap;
    if (initialize_bitmap(&bitmap,rows,cols)==NULL)
    {
        showError(IALLOC);
        free(params.file);
        free_bitmap(&bitmap);
        fclose(f);
        return EXIT_FAILURE;
    }
    bool load = load_bitmap(&bitmap,f);
    if (params.state==TEST)
    {
        if (load) printf("Valid\n");
            else printf("Invalid\n");
    }
    else if (!load)
    {
        printf("Invalid\n");
        free(params.file);
        free_bitmap(&bitmap);
        fclose(f);
        return EXIT_FAILURE;
    }
    if (params.state==VLINE)
    {
        find_vline(&bitmap,&x1,&y1,&x2,&y2);
        printf("%d %d %d %d\n",x1,y1,x2,y2);
    }
    else if (params.state==HLINE)
    {
        find_hline(&bitmap,&x1,&y1,&x2,&y2);
        printf("%d %d %d %d\n",x1,y1,x2,y2);
    }
    else if (params.state==SQUARE)
    {
        find_square(&bitmap,&x1,&y1,&x2,&y2);
        printf("%d %d %d %d\n",x1,y1,x2,y2);
    }
    free(params.file);
    free_bitmap(&bitmap);
    fclose(f);
  return EXIT_SUCCESS;
}
