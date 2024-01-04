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
//Init values of a vector of size Rows.
void InitVectorInt(int *pVector, const int Rows)
{ 
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%100;
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
//Sums x to Vector
void IntSumVectorInt(int *pVector, const int Rows, const int dato)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=pVector[i]+dato;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int x;  //Integer to Sum.
 int * pVector;


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
 
 //Init x
 x=lrand48()%9+1;
 printf("x=%d.\n",x);

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Init values of Vector
 InitVectorInt (pVector,Rows);
 PrintVectorInt(pVector,Rows,"Vector");
 
 
 IntSumVectorInt(pVector,Rows,x);
 
 PrintVectorInt(pVector,Rows,"Vector + x");
 
 free((void *)pVector);
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
