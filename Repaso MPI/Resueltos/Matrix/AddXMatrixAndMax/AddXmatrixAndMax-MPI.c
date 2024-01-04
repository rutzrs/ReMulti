#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
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
//Init values of a matrix of size Rows.
void InitMatrixInt(int *pMatrix, const int Size)
{ 
 for (int i=0;i<Size;i++)
     pMatrix[i]=lrand48()%100;
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
//Print a matrix of size Rows
void PrintMatrixInt(int *pMatrix, const int Size, const int Cols, char * String)
{
 printf("%s -----------------------------------------\n", String);
 for (int i=0;i<Size;i++){
     printf("%2d,", pMatrix[i]);
     if ((i+1)%Cols==0) printf("\n");
 }
 puts("");
}

/*---------------------------------------------------------------------------*/
void AddXmatrixAndMax(int *pMatrix, const int Size, const int dato, int * Max)
{
 
 for (int i=0;i<Size;i++){
   pMatrix[i]+=dato;
 }
      
 int LocalMax=pMatrix[0];
 for (int i=1;i<Size;i++){
   LocalMax = LocalMax > pMatrix[i] ? LocalMax : pMatrix[i];
 }
 *Max=LocalMax;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows, Cols;
 int * pMatrix=NULL, Max=0, x;
 int * pSubMatrix=NULL;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int *pPosSubM;			//Vector with position each task starts in Vectors.
 int *pSizeSubM;			//Vector with size of subvector.
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
        srand48(MyRank); 
        //Init x
	x=lrand48()%9+1;  //+1 to avoid x=0.
	printf("x=%d.\n",x);
	printf("Rows=%d. Cols=%d\n", Rows, Cols);
 }

 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,Root,MPI_COMM_WORLD);
 MPI_Bcast(&Cols,1,MPI_INT,Root,MPI_COMM_WORLD);
 MPI_Bcast(&x,1,MPI_INT,Root,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and pSizeSubV
 pPosSubM = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacM");
 pSizeSubM = (int *)GetMem(NTasks,sizeof(int),"All tasks:pSizeSubM");
 
 Rem=(Rows*Cols)%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubM[i] = (Rows*Cols)/NTasks;
      if (Rem > 0) 
         {
          pSizeSubM[i]++;
          Rem--;
         }

     pPosSubM[i] = AddPos;
     AddPos += pSizeSubM[i];
    }
    
 srand48(MyRank);
 
 #if (PRINT==1)
 printf("Task %d out of %d, responsible elements: [%d,%d].\n",
      MyRank,NTasks,pPosSubM[MyRank],pPosSubM[MyRank]+pSizeSubM[MyRank]-1);
 #endif
  

 //Only workers needs psubvector3
 if (MyRank!=Root)
    {
     pSubMatrix  = (int *) GetMem(pSizeSubM[MyRank],sizeof(int),"Main:SubMatrix");
     InitMatrixInt (pSubMatrix,pSizeSubM[MyRank]);
     
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubMatrix,pSizeSubM[MyRank],"SubMatrix:");
     #endif    
     
     for (int i=0;i<pSizeSubM[MyRank];i++){
         pSubMatrix[i]+=x;
     }
      
     int LocalMax=-999;
     for (int i=0;i<pSizeSubM[MyRank];i++){
         LocalMax = LocalMax > pSubMatrix[i] ? LocalMax : pSubMatrix[i];
     }
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubMatrix,pSizeSubM[MyRank],"SubMatrix:");
     #endif
     
     MPI_Gatherv(pSubMatrix, pSizeSubM[MyRank], MPI_INT,
                 pMatrix,    pSizeSubM, pPosSubM, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(&LocalMax, &Max, 1, MPI_INT, MPI_MAX, Root, MPI_COMM_WORLD);
    }

 //Take care: pSubVector1 and 2 starts in 0.
 //Root task do not need pSubvector3, it use pVector3 IN_PLACE 
 if (MyRank==Root)
    {
     pMatrix  = (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matrix");
     InitMatrixInt (pMatrix,Rows*Cols);
      
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pMatrix,pSizeSubM[MyRank],"SubMatrix:");
     #endif
         
     for (int i=pPosSubM[Root];i<pPosSubM[Root]+pSizeSubM[MyRank];i++){
         pMatrix[i]+=x;
     }
     
     for (int i=pPosSubM[Root];i<pPosSubM[Root]+pSizeSubM[MyRank];i++){
         Max = Max > pMatrix[i] ? Max : pMatrix[i];
     }
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pMatrix,pSizeSubM[MyRank],"SubMatrix:");
     #endif
     MPI_Gatherv(MPI_IN_PLACE,0, MPI_INT,
                 pMatrix, pSizeSubM, pPosSubM, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(MPI_IN_PLACE, &Max, 1, MPI_INT, MPI_MAX, Root, MPI_COMM_WORLD);
    }


 //All deallocate subvectors
 free((void *)pSizeSubM);
 free((void *)pPosSubM);
 free((void *)pSubMatrix);

 if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintMatrixInt(pMatrix,Rows*Cols, Cols,"Matrix");
     #endif
     free((void *)pMatrix);
     printf("Max=%d.\n",Max);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
