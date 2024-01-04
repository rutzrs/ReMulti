#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define PRINT 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Init values of a vector of size Rows.
void InitVectorInt(int *pVector, const int Rows)
{ 
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%10;
}

/*---------------------------------------------------------------------------*/
//Print a vector of size Rows
void PrintVectorInt(int *pVector, const int Rows, char * String)
{
 printf("%s -----------------------------------------\n", String);
 for (int i=0;i<Rows;i++)
     printf("%2d,", pVector[i]);
 puts("");
}

/*---------------------------------------------------------------------------*/
void ProdXvectorAndMax(int *pVector, const int Rows, const int dato, int * Max)
{
 
 for (int i=0;i<Rows;i++)
      pVector[i]*=dato;
      
 int LocalMax=pVector[0];
 for (int i=1;i<Rows;i++)
      LocalMax = LocalMax > pVector[i] ? LocalMax : pVector[i];
 *Max=LocalMax;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector, Max, x;


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

 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Init values of Vector
 InitVectorInt (pVector,Rows);
 
 //Init x
 x=lrand48()%9+1;  //+1 to avoid x=0.
 printf("x=%d.\n",x);

 PrintVectorInt(pVector,Rows,"Vector");
 
 ProdXvectorAndMax(pVector,Rows,x,&Max);
 
 PrintVectorInt(pVector,Rows,"Vector+x");
 
 printf("Max=%d.\n",Max);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
