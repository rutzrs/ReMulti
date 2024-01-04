#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
 #if (DEBUG==2)
 clock_t c0,c1;
 c0=clock(); 
 #endif
 
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%10;
  
 #if (DEBUG==2)
 c1=clock();
 printf("T. Init Vector= %g sec.\n",(float)(c1-c0)/CLOCKS_PER_SEC);
 #endif
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
void SumVectorInt(int *pVector, const int Rows, int *pSum)
{
 int Sum=0;
 
 for (int i=0;i<Rows;i++)
     Sum+=pVector[i];

 *pSum=Sum;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector;
 int Sum=0;

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
 printf("SumVectorInt: Rows=%d.\n", Rows);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 InitVectorInt (pVector,Rows);
 
 #if (DEBUG==1)
 PrintVectorInt(pVector,Rows);
 #endif
 
 SumVectorInt  (pVector,Rows, &Sum);
 
 printf("SumVectorInt=%d.\n",Sum);
 
 
 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
