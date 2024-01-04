#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define DEBUG 1


typedef struct th_sumv_args
{
 int *		pVector;
 int 		Rows;
 int		rows_start;
 int 		n_sub_rows;
 int *		pSum;
 pthread_mutex_t * pth_Sum_mutex;
 int 		ThreadId;
} Type_Th_SumV_Args;


/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -nt <n trheads>			");
 exit(0);
}

//-----------------------------------------------------------------------------
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
void * SumVectorIntPTh(void * pTh_Args)
{
 Type_Th_SumV_Args * pMyData;
 pMyData=(Type_Th_SumV_Args *) pTh_Args;
 
 int Sum=0;
 int rows_start, rows_end;
 int * pVector;
 
 pVector	= pMyData->pVector;
 rows_start	= pMyData->rows_start;
 rows_end	= pMyData->rows_start + pMyData->n_sub_rows;
 
  #if (DEBUG==1)
 printf("I am thread %d. Doing rows [%d,%d].\n", 
        pMyData->ThreadId, 
        pMyData->rows_start, 
        rows_end-1);
 #endif
 for (int i=rows_start; i<rows_end; i++)
	 Sum+=pVector[i];
	 
 pthread_mutex_lock(pMyData->pth_Sum_mutex);
 (*pMyData->pSum)+=Sum;
 pthread_mutex_unlock(pMyData->pth_Sum_mutex);
 
 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector;
 int NThreads; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory. 
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_SumV_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 int Sum=0;
 pthread_mutex_t pth_Sum_mutex; //To update Sum
 int rc;  						//Returned value;
 void *status;					//Status from thread.

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

 #if (DEBUG==1)
 printf("SumVectorInt-PTh: Rows=%d, NThreads=%d \n", Rows, NThreads);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 /* Initialize mutex objects*/
 pthread_mutex_init(&pth_Sum_mutex, NULL);

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Get mem for threads
 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t), 
                                 "Main: pThreads");
 pTh_Args = (Type_Th_SumV_Args *) GetMem(NThreads, 
                                            sizeof(Type_Th_SumV_Args),
                                            "Main: pTh_Data");
 
 InitVectorInt (pVector,Rows);
 
 #if (DEBUG==1)
 PrintVectorInt(pVector,Rows);
 #endif
 
  for (int t=0; t<NThreads; t++) 
     {
 	  HowtoShareVector(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pVector		= pVector;
 	  pTh_Args[t].Rows			= Rows;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].pSum		= &Sum;
 	  pTh_Args[t].pth_Sum_mutex	= &pth_Sum_mutex;
 	  pTh_Args[t].ThreadId		= t;
 
      #if (DEBUG==1)
      printf("Thread %d out of %d, responsible of %d rows: [%d,%d].\n",
             t,NThreads,n_sub_rows, rows_start, rows_start+n_sub_rows-1);
      #endif
      
      //By default threads are joinable.
     rc = pthread_create(&pThreads[t],NULL, 
                          SumVectorIntPTh, (void *)&pTh_Args[t]);
     }
 
 //Do the threads join
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

 printf("SumVectorInt=%d.\n",Sum);
  
 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void *)pVector);
 pthread_mutex_destroy(&pth_Sum_mutex);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
