#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define DEBUG 1


typedef struct th_medv_args
{
 int **		pMatrix;
 int 		Rows;
 int *		media;
 int 		Cols;
 int		rows_start;
 int 		n_sub_rows;
 int *		pSum;
 int 		ThreadId;
} Type_Th_MedV_Args;


/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 puts("\t  -c <n cols>			");
 puts("\t  -nt <n trheads>			");
 exit(0);
}

//-----------------------------------------------------------------------------
void HowtoShareMatrix(int ThisTh, int NTh, int NRows, int *start, int *n)
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
void InitMatrixInt(int **pMatrix, const int Rows, const int Cols)
{
 #if (DEBUG==2)
 clock_t c0,c1;
 c0=clock();
 #endif

 for (int i=0;i<Rows;i++)
    for (int j=0;j<Cols;j++)
     pMatrix[i][j]=lrand48()%100;

 #if (DEBUG==2)
 c1=clock();
 printf("T. Init Matrix= %g sec.\n",(float)(c1-c0)/CLOCKS_PER_SEC);
 #endif
}

/*---------------------------------------------------------------------------*/
void PrintMatrixInt(int **pMatrix, const int Rows, const int Cols)
{
 puts("-----------------------------------------");
 for (int i=0;i<Rows;i++){
   printf("[");
   for (int j=0;j<Cols;j++) {
     printf("%d,", pMatrix[i][j]);
   }
   printf("]\n");
 }
 puts("");
}

/*---------------------------------------------------------------------------*/
void * MediaMatrixIntPTh(void * pTh_Args)
{
 Type_Th_MedV_Args * pMyData;
 pMyData=(Type_Th_MedV_Args *) pTh_Args;

 int sum=0;
 int rows_start, rows_end;
 int ** pMatrix;

 pMatrix = pMyData->pMatrix;
 rows_start = pMyData->rows_start;
 rows_end = rows_start + pMyData->n_sub_rows;

  #if (DEBUG==1)
 printf("I am thread %d. Doing rows [%d,%d].\n",
        pMyData->ThreadId,
        pMyData->rows_start,
        rows_end-1);
 #endif
 //Bucle

 for (int i=rows_start;i<rows_end;i++)
    for (int j=0;j<pMyData->Cols;j++)
     sum+=pMatrix[i][j];

 *pMyData->pSum+=sum;
 *pMyData->media= *pMyData->pSum/(pMyData->Rows*pMyData->Cols);


 pthread_exit((void *)&(pMyData->ThreadId));
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int Cols;
 int **pMatrix;
 int NThreads;
 int media=0; 					//Number of threads.
 pthread_t	*pThreads=NULL; 	//Vector of threads. Dynamic memory.
 //Vector of structs with data to threads. Dinamic memory allocation.
 Type_Th_MedV_Args *pTh_Args=NULL;
 int rows_start,n_sub_rows; 	//Thread start at and how many.
 int Sum=0;
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

    if (!ExistArg("-c",argc,argv))
       {
       fputs("Parameter -c is neccesary.\n",stderr);
       ParametersError();
       }
    else
       {
        Cols = atoi(GetArg("-c",argc,argv));
        if (Cols <1)
           {
            puts("Cols<1");
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
 printf("SumMatrixInt-PTh: Rows=%d\tCols=%d.\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));


 //Calloc de Getmen put data to zeroes
 pMatrix  = (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Vector");

 //Get mem for threads
 pThreads = (pthread_t *) GetMem(NThreads, sizeof(pthread_t),
                                 "Main: pThreads");
 pTh_Args = (Type_Th_MedV_Args *) GetMem(NThreads,
                                            sizeof(Type_Th_MedV_Args),
                                            "Main: pTh_Data");

 InitMatrixInt (pMatrix,Rows,Cols);

 #if (DEBUG==1)
 PrintMatrixInt(pMatrix,Rows,Cols);
 #endif

  for (int t=0; t<NThreads; t++)
     {
 	  HowtoShareMatrix(t,NThreads, Rows, &rows_start, &n_sub_rows);
 	  pTh_Args[t].pMatrix		= pMatrix;
 	  pTh_Args[t].Rows		= Rows;
   	  pTh_Args[t].Cols		= Cols;
   	  pTh_Args[t].media		= &media;
 	  pTh_Args[t].rows_start	= rows_start;
 	  pTh_Args[t].n_sub_rows	= n_sub_rows;
 	  pTh_Args[t].pSum		= &Sum;
 	  pTh_Args[t].ThreadId		= t;

      #if (DEBUG==1)
      printf("Thread %d out of %d, responsible of %d rows: [%d,%d].\n",
             t,NThreads,n_sub_rows, rows_start, rows_start+n_sub_rows-1);
      #endif

      //By default threads are joinable.
     rc = pthread_create(&pThreads[t],NULL,
                          MediaMatrixIntPTh, (void *)&pTh_Args[t]);
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
     printf("MediaMatrixInt=%d.\n",media);

 //Free allocated memory
 free((void *)pTh_Args);
 free((void *)pThreads);
 free((void **)pMatrix);


 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
