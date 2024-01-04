#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"


#define PRINT 1

//To avoid repeating struct drand48_data.
typedef struct drand48_data StDrand48Data;

//I'll use the same structure to init and to sum the vector.
typedef struct th_args
{
 int *		pVector1;
 int *		pVector2;
 int *		pVector3;
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 StDrand48Data   * pRandData;
 int 		ThreadId;
} Type_Th_Args;

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -nt <n trheads>			");
 exit(0);
}
/*---------------------------------------------------------------------------*/
void HowtoShareVector(int ThisTh, int NTh, int NRows, int *start, int *n)
{
 int n_sub_rows= NRows/NTh;			//How many sub rows.
 int n_larger_threads= NRows%NTh;	//Threads with more rows
 int row_start;						//In which row a thread starts.

 if (ThisTh < n_larger_threads)
    { 
     n_sub_rows = n_sub_rows + 1;
     row_start = 0 + ThisTh * n_sub_rows;
    }  
 else 
    if (n_sub_rows > 0)
       row_start = n_larger_threads + ThisTh * n_sub_rows;
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
void * InitVectorIntPTh(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 long RandNumber;  
 int rows_start, rows_end;
 int * pVector1;
 int * pVector2;
 

 
 pVector1	= pMyData->pVector1;
 pVector2	= pMyData->pVector2; 
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Initiating rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 {
	  lrand48_r(pMyData->pRandData,&RandNumber);
	  pVector1[i]=RandNumber%10;
	  lrand48_r(pMyData->pRandData,&RandNumber);
	  pVector2[i]=RandNumber%10;
	 }
 pthread_exit((void *)&(pMyData->ThreadId));
}
/*---------------------------------------------------------------------------*/
//Max{Vector1,Vector2} -> Vector3
void * Max2VectorInt(void * pTh_Args)
{
 Type_Th_Args * pMyData;
 pMyData=(Type_Th_Args *) pTh_Args;
 
 int rows_start, rows_end;
 int * pVector1, * pVector2, * pVector3;
 
 pVector1	= pMyData->pVector1;
 pVector2	= pMyData->pVector2;
 pVector3	= pMyData->pVector3;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
 #if (PRINT==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 
 for (int i=rows_start;i<rows_end;i++)
     pVector3[i]=(pVector1[i]>pVector2[i])?pVector1[i]:pVector2[i];
     
  pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int *pVector1, *pVector2, *pVector3;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 void *status;					//Status from thread.
  StDrand48Data *pRandData=NULL; 				//drand48_data[t] for each thread.

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
         puts("NThreads must be > 0 and >= Rows.");
         exit(1);
        }
    }  

 #if (PRINT==1)
 printf("Rows=%d.\n", Rows);
 #endif

 //Init seed of random number generator
 //srand48(0);
//Get mem for pRandData
 pRandData=(StDrand48Data *) GetMem(NThreads, sizeof(StDrand48Data), "pRandomData");
 //Init seeds
 for (int i=0;i<NThreads;i++)
 	 srand48_r(i,&pRandData[i]);

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");
 
 //Get mem for threads
 pThreads = (pthread_t *)    GetMem(NThreads, sizeof(pthread_t), 
                                    "Main: pThreads");
 pTh_Args = (Type_Th_Args *) GetMem(NThreads, sizeof(Type_Th_Args),
                                    "Main: pTh_Data");

 //==========================================================================
 
 //InitVector in parallel.
   for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pVector1		= pVector1;
 	  pTh_Args[t].pVector2		= pVector2;
 	  pTh_Args[t].Rows			= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].pRandData  	= &pRandData[t];
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].ThreadId		= t;

      //By default threads are joinable.
      pthread_create(&pThreads[t],NULL, InitVectorIntPTh,(void *)&pTh_Args[t]);
     }

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

 #if (PRINT==1)
 PrintVInt(stdout,pVector1,Rows);
 PrintVInt(stdout,pVector2,Rows);
 #endif

 //===========================================================================

  for (int t=0; t<NThreads; t++) 
     {
      	  pTh_Args[t].pVector3		= pVector3;
      pthread_create(&pThreads[t],NULL,Max2VectorInt, (void *)&pTh_Args[t]);
     }

 //Do the threads join
 for (int t=0;t<NThreads; t++)
     {
      pthread_join(pThreads[t], (void **) &status);
      #if (PRINT==1)
      printf("Main: join thread %d with status = %d.\n",t,*(int *)status);
      fflush(stdout);
      #endif
     }

 
 #if (PRINT==1)
 PrintVInt(stdout,pVector3,Rows);
 #endif
 
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
