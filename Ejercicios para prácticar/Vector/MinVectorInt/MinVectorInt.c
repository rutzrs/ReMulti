#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define DEBUG 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
void InitVectorInt(int *pVector, const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%10;
}

/*---------------------------------------------------------------------------*/
void PrintVectorInt(int *pVector, const int Rows)
{
 puts("-----------------------------------------");
 for (int i=0;i<Rows;i++)
     printf("%d,", pVector[i]);
 puts("");
}

/*---------------------------------------------------------------------------*/
void MinVectorInt(int *pVector, const int Rows, int *pMin)
{
 int Min=pVector[0];
 
 for (int i=0;i<Rows;i++)
     if (pVector[i] < Min) Min = pVector[i];

 *pMin=Min;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector;
 int Min=0;

 if (ExistArg("-h",argc,argv))
    ParametersError();  

 if (!ExistArg("-r",argc,argv))
    {
    fputs("Parameter -r is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     Rows = atoi(GetArg("-r",argc,argv));
     if (Rows <1)
        {
         puts("Rows<1");
         exit(1);
        }
    } 

 #if (DEBUG==1)
 printf("MinVectorInt: Rows=%d.\n", Rows);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 InitVectorInt (pVector,Rows);
 PrintVectorInt(pVector,Rows);
 MinVectorInt  (pVector,Rows, &Min);
 
 printf("MinVectorInt=%d.\n",Min);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
