/*
Progam that generate two vectors (1 and 2)  of random ints in [0,9] and performs vector3[i]=min{vector1[i],vector2[i]}.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"
#include <pthread.h>

#define DEBUG 0
#define PRINT 0


typedef struct th_minv_args
{
 int *		pVector1;
 int *		pVector2;
 int *		pVector3;
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 int 		ThreadId;
} Type_Th_MinV_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 exit(0);
}

void HowtoShareVector(int ThisTh, int NTh, int NRows, int *start, int *n)
{
 int n_sub_rows= NRows/NTh;	//How many sub rows.
 int n_larger_threads= NRows-NTh*n_sub_rows;	//Threads with more rows
 int row_start;	//In which row a thread starts.

 if (ThisTh < n_larger_threads)
    { 
     n_sub_rows = n_sub_rows + 1;
     row_start = 0 + ThisTh * n_sub_rows;
    }  
 else 
    if (n_sub_rows > 0)
    { 
     row_start = 0 + n_larger_threads + ThisTh * n_sub_rows;
    }
 else 
    { // this process has only zero elements
     row_start = -1;
     n_sub_rows  =  0;
    }

 *n		= n_sub_rows;
 *start	= row_start;
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
     printf("%3d,", pVector[i]);
 puts("");
}

/*---------------------------------------------------------------------------*/
//Sum Vector1 and Vector2 in Vector3
void * Min2VectorInt(void * pTh_Args)
{
 Type_Th_MinV_Args * pMyData;
 pMyData=(Type_Th_MinV_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int * pVector1, * pVector2, * pVector3;
 
 pVector1	= pMyData->pVector1;
 pVector2	= pMyData->pVector2;
 pVector3	= pMyData->pVector3;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (DEBUG==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 
 for (int i=rows_start;i<rows_end;i++)
     pVector3[i]=(pVector1[i]<pVector2[i])?pVector1[i]:pVector2[i];

 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1, * pVector2, * pVector3;
 int NThreads; //Numero d ehebras
 pthread_t *pThreads=NULL; //Vector de hebras generado mediante alojamiento dinamico de memoria
 Type_Th_MinV_Args *pTh_Args=NULL;
 int rows_start; //Donde empieza una hebra
 int n_sub_rows; //Carga de trabajo de cada hebra
 int rc; //Return value
 void *status; //Estado de cada hebra

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
 if (!ExistArg("-nt",argc,argv))
    {
    fputs("Parameter -nt is neccesary.\n",stderr);
    ParametersError();
    }
 else
    {
     NThreads = atoi(GetArg("-nt",argc,argv)); 
     if (NThreads <0 || NThreads > Rows)
        {
         puts("NThreads must be > 0 and <= Rows.");
         exit(1);
        }
    }  
    
 printf("Min2VectorInt-PTh: Rows=%d, NThreads=%d \n", Rows, NThreads);

 //Init seed of random number generator
 srand48(time(NULL));
 
  

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");

 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t), 
                                 "Main: pThreads");
 pTh_Args = (Type_Th_MinV_Args *) GetMem(NThreads, 
                                            sizeof(Type_Th_MinV_Args),
                                            "Main: pTh_Data");
                                             
 //Init values of Vector1 and 2
 InitVectorInt (pVector1,Rows);
 InitVectorInt (pVector2,Rows);

 # if (PRINT==1)
 PrintVectorInt(pVector1,Rows,"Vector1");
 PrintVectorInt(pVector2,Rows,"Vector2");
 # endif
 
  for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
	  pTh_Args[t].pVector1		= pVector1;
	  pTh_Args[t].pVector2		= pVector2;
	  pTh_Args[t].pVector3		= pVector3;
 	  pTh_Args[t].Rows		= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;
 
      #if (DEBUG==1)
      printf("Thread %d out of %d, responsible of %d rows: [%d,%d].\n",
             t,NThreads,n_sub_rows, rows_start, rows_start+n_sub_rows-1);
      #endif
      
      
	rc = pthread_create(&pThreads[t],NULL, 
                          Min2VectorInt, (void *)&pTh_Args[t]);
     }

 for (int t=0;t<NThreads; t++)
     {
     rc = pthread_join(pThreads[t], (void **) &status);
      if (rc) 
         {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
      #if (DEBUG==1)
      printf("Main: join thread %d with status = %d.\n",
             t,*(int *)status);
      fflush(stdout);
      #endif
     } 
 
 # if (PRINT==1)
 PrintVectorInt(pVector3,Rows,"Min{Vector1,Vector2}");
 # endif
 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
