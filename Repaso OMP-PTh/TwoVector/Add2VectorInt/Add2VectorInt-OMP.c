#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#ifdef _OPENMP
#  include <omp.h>
#endif

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
//Sum Vector1 and Vector2 in Vector3
void Sum2VectorInt(int *pVector1, int *pVector2, int * pVector3,
                   const int Rows)
{
#ifdef OPENMP
#pragma omp parallel for default(none) shared(pVector1,pVector2,pVector3,Rows)
#endif
 for (int i=0;i<Rows;i++)
     pVector3[i]=pVector1[i]+pVector2[i];
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1, * pVector2, * pVector3;


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

 # ifdef _OPENMP
 #   pragma omp parallel default(none)
     { 
 #     pragma omp single 
      printf("OpenMP-parallel with %1d threads\n", omp_get_num_threads());
     } /* end omp parallel */
 # endif
 
 //Init seed of random number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
 
 //Init values of Vector1 and 2
 InitVectorInt (pVector1,Rows);
 InitVectorInt (pVector2,Rows);


 PrintVectorInt(pVector1,Rows,"Vevtor1");
 PrintVectorInt(pVector2,Rows,"Vector2");
 
 Sum2VectorInt(pVector1,pVector2,pVector3,Rows);
 
 PrintVectorInt(pVector3,Rows,"Vector1+Vector2");
 
 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
