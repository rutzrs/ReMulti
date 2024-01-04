#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/times.h>
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
void InitMatrixMin(int *pMatrix, const int Size)
{
 for (int i=0;i<Size;i++) {
	pMatrix[i]=lrand48()%100;
 }
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
void PrintMatrixMin(int *pMatrix, const int Size, const int Cols)
{
 puts("-----------------------------------------");
 for (int i=0;i<Size;i++) {
	printf("%d,", pMatrix[i]);
	if ((i+1)%Cols==0) printf("\n");	 
 }
 puts("");
}
/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols, Min=9999;
 int * pMatrix=NULL, * pSubMatrix=NULL;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int *pPosSubV;			//Vector with position each task starts in Vectors.
 int *pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.
 int Root=0;			//Root process.

 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
 if (MyRank==Root)	//Task Root--
    {
     #if (PRINT==1)
     printf("Task %d is the Root.\n",MyRank);
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

	 #if (PRINT==1)
	 printf("Rows=%d Cols=%d.\n", Rows, Cols);
	 #endif
 }
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,Root,MPI_COMM_WORLD);
 MPI_Bcast(&Cols,1,MPI_INT,Root,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and pSizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:pSizeSubV");
 
 Rem=(Rows*Cols)%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = (Rows*Cols)/NTasks;
      if (Rem > 0) 
         {
          pSizeSubV[i]++;
          Rem--;
         }

     pPosSubV[i] = AddPos;
     AddPos += pSizeSubV[i];
    }

 #if (PRINT==1)
 printf("Task %d out of %d, responsible elements: [%d,%d].\n",
      MyRank,NTasks,pPosSubV[MyRank],pPosSubV[MyRank]+pSizeSubV[MyRank]-1);
 #endif
 
 //Init seed of random number generator to the task rank
 //each sequence is local to each task becasue tasks do not share variables.
 srand48(MyRank);
  
 //Only workers needs psubvector3
 if (MyRank!=Root)
    {
     pSubMatrix  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),"SubMatrix");
     InitMatrixMin (pSubMatrix,pSizeSubV[MyRank]);
     
     int LocalMin=9999;
     for (int i=0;i<pSizeSubV[MyRank];i++)
         if (pSubMatrix[i] < LocalMin) LocalMin = pSubMatrix[i];
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubMatrix,pSizeSubV[MyRank],"SubVector:");
     #endif
     
     MPI_Gatherv(pSubMatrix, pSizeSubV[MyRank], MPI_INT,
                 pMatrix,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(&LocalMin,&Min,1,MPI_INT,MPI_MIN,Root,MPI_COMM_WORLD);
    }

 //Take care: pSubVector1 and 2 starts in 0.
 //Root task do not need pSubvector3, it use pVector3 IN_PLACE 
 if (MyRank==Root)
    {
     pMatrix  = (int *) GetMem(Rows*Cols,sizeof(int),"SubMatrix");
     InitMatrixMin (pMatrix,pSizeSubV[MyRank]);
     
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[Root];i++)
         if (pMatrix[i] < Min) Min = pMatrix[i];
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pMatrix,pSizeSubV[MyRank],"SubVector:");
     #endif
     
     MPI_Gatherv(MPI_IN_PLACE,0, MPI_INT,
                 pMatrix, pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(MPI_IN_PLACE,&Min,1,MPI_INT,MPI_MIN,Root,MPI_COMM_WORLD);
    }


 //All deallocate subvectors
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatrix);

 if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintMatrixMin(pMatrix,Rows*Cols,Cols);
     #endif
     free((void *)pMatrix);
     printf("Min=%d.\n", Min);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
