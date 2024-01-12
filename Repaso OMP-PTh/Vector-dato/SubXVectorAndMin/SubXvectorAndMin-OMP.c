#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>

#define PRINT 1
#define TIMES 1

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
 struct drand48_data RandData; 
 long Random;
 srand48_r(omp_get_thread_num(), &RandData);
 
 #pragma omp for
 for (int i=0;i<Rows;i++) {
     lrand48_r(&RandData,&Random);
     pVector[i]=Random%100;
 }
 }
}

/*---------------------------------------------------------------------------*/
void SubXvectorAndMin(int *pVector, const int Rows, const int dato, int * Min)
{
 #pragma omp parallel for reduction(-:pVector[:Rows]) 
 for (int i=0;i<Rows;i++)
      pVector[i]-=dato;
      
 int LocalMin=pVector[0];
 #pragma omp parallel for reduction(min:LocalMin)
 for (int i=1;i<Rows;i++)
      LocalMin = LocalMin < pVector[i] ? LocalMin : pVector[i];
      
 *Min=LocalMin;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector, Min, x;

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

 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 srand48(time(NULL));
 
  #pragma omp parallel default(none) shared(Rows)
     { 
 	  #pragma omp single 
      printf("SumVectorInt OMP with %d threads, Rows=%d\n", 
      		 omp_get_num_threads(),Rows);
     } 

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 InitVectorInt (pVector,Rows);

 //Init x
 x=lrand48()%9+1;  //+1 to avoid x=0.
 printf("x=%d.\n",x);
 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector");
 #endif
 
 SubXvectorAndMin(pVector,Rows,x,&Min);
 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector-x");
 #endif
 
 printf("Min=%d.\n",Min);

 free((void *)pVector);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
