#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>

#define PRINT 0
#define DEBUG 0

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
#pragma omp parallel default(none) shared(pVector, Rows)
{
 #pragma omp for
 for (int i=0;i<Rows;i++) 
 {
     pVector[i]=lrand48()%10;
 }
 }
}


/*---------------------------------------------------------------------------*/
//Max{Vector1,Vector2} -> Vector3
void Max2VectorInt(int *pVector1, int *pVector2, int * pVector3,
                   const int Rows)
{
#pragma omp parallel for default(none) shared(pVector1, pVector2, pVector3, Rows)
 for (int i=0;i<Rows;i++)
     pVector3[i]=(pVector1[i]>pVector2[i])?pVector1[i]:pVector2[i];
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1, * pVector2, * pVector3;

 //IMPORTANT: This program only works for OPENMP.
 #ifndef _OPENMP
 printf("Compilado sin -fopenmp.\n");
 exit(1);
 #endif
 
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

 #if (PRINT==1)
 printf("Rows=%d.\n", Rows);
 #endif
 
 #pragma omp parallel default(none) shared(Rows)
     { 
 	  #pragma omp single 
      printf("SumVectorInt OMP with %d threads, Rows=%d\n", 
      		 omp_get_num_threads(),Rows);
     } 

 //Init seed of random number generator
 srand48(0);

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
 
 //Init values of Vector1 and 2
 InitVectorInt (pVector1,Rows);
 InitVectorInt (pVector2,Rows);
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector1,Rows);
 PrintVInt(stdout,pVector2,Rows);
 #endif

 Max2VectorInt(pVector1,pVector2,pVector3,Rows);
 
 #if (PRINT==1)
 PrintVInt(stdout,pVector3,Rows);
 #endif
 
 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
