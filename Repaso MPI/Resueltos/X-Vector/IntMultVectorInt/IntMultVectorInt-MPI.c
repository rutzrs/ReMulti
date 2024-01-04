#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

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
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%100;
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
//Mults x to Vector
void IntMultVectorInt(int *pVector, const int Rows, const int dato)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=pVector[i]*dato;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int x;  //Integer to Mult.
 int * pVector=NULL, * pSubVector=NULL;
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
	srand48(Root);
	//Init x
	x=lrand48()%9+1;
	printf("x=%d.\n",x);
	
 	printf("Rows=%d.\n", Rows);
 }
 
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,Root,MPI_COMM_WORLD);
  MPI_Bcast(&x,1,MPI_INT,Root,MPI_COMM_WORLD);
 //Get men and initialize DisplacV and pSizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:pSizeSubV");
 
 Rem=Rows%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = Rows/NTasks;
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
 
 //Only workers needs psubvector3
 if (MyRank!=Root)
    {
     srand48(MyRank);
     pSubVector  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),"SubVector");
     InitVectorInt (pSubVector,pSizeSubV[MyRank]);
     for (int i=0;i<pSizeSubV[MyRank];i++)
	 pSubVector[i]*=x;
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubVector,pSizeSubV[MyRank],"SubVector3:");
     #endif
     
     MPI_Gatherv(pSubVector, pSizeSubV[MyRank], MPI_INT,
                 pVector,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
    }

 //Take care: pSubVector1 and 2 starts in 0.
 //Root task do not need pSubvector3, it use pVector3 IN_PLACE 
 if (MyRank==Root)
    {
     pVector  = (int *) GetMem(Rows,sizeof(int),"SubVector");
     InitVectorInt (pVector,pSizeSubV[MyRank]);
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[Root];i++)
	 pVector[i]*=x;
         
     MPI_Gatherv(MPI_IN_PLACE,0, MPI_INT,
                 pVector, pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
    }


 //All deallocate subvectors
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector);

 if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintVectorInt(pVector,Rows,"Vector");
     #endif
     free((void *)pVector);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
