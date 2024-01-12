#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <omp.h>

#define PRINT 0

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
 #pragma omp parallel default(none) shared(pVector,Rows)
 {
  //Inside a omp paralel: These variables are local to each thread.
  struct drand48_data RandData; 
  long Random;
  srand48_r(omp_get_thread_num(), &RandData);

  #pragma omp for 
  for (int i=0;i<Rows;i++)
  	  {
  	   lrand48_r(&RandData,&Random);
       pVector[i]=Random%100;
      }
 }
}

/*---------------------------------------------------------------------------*/
//Adds x to Vector
void IntAddVectorInt(int *pVector, const int Rows, const int dato)
{
  #pragma omp parallel for
 for (int i=0;i<Rows;i++)
     pVector[i]=pVector[i]+dato;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int x;  //Integer to Add.
 int * pVector;

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
 printf("SumVectorInt: Rows=%d.\n", Rows);
 #endif
 
  #pragma omp parallel default(none) shared(Rows)
     { 
 	  #pragma omp single 
      printf("IntAddVectorInt OMP with %d threads, Rows=%d\n", 
      		 omp_get_num_threads(),Rows);
     } 
 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 srand48(time(NULL));
 
 //Init x
 x=lrand48()%10;
 printf("x=%d.\n",x);

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Init values of Vector
 InitVectorInt (pVector,Rows);
 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector  ");
 #endif
 
 IntAddVectorInt(pVector,Rows,x);

 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector+x");
 #endif
 
 free((void *)pVector);
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
