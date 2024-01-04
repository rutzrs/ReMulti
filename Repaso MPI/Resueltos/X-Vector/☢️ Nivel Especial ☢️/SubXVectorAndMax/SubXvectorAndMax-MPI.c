#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>
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
void SubXvectorAndMax(int *pVector, const int Rows, const int dato, int * Max)
{
 for (int i=0;i<Rows;i++)
      pVector[i]-=dato;

 int LocalMax=pVector[0];
 for (int i=0;i<Rows;i++)
      LocalMax = LocalMax > pVector[i] ? LocalMax : pVector[i];
 
 *Max=LocalMax;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector=NULL, *pSubVector=NULL, Max = -9999, x;
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
    
    srand48(MyRank);
    //Init x
    x=lrand48()%9+1;  //+1 to avoid x=0.
    printf("x=%d.\n",x);
    
    printf("Rows=%d.\n", Rows);
    //pVector  = (int *) GetMem(Rows,sizeof(int),"Vector");
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

 if (MyRank!=Root)
 	srand48(MyRank);
 	
 //Only workers needs psubvector
 
 if (MyRank!=Root)
    {
     pSubVector  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),"SubVector");
     InitVectorInt (pSubVector,pSizeSubV[MyRank]);
     
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVI(stdout,pSubVector,pSizeSubV[MyRank],"Subvector Antes:");
     #endif
     
     for (int i=0;i<pSizeSubV[MyRank];i++)
      	pSubVector[i]-=x;

     int LocalMax=pSubVector[0];
     for (int i=0;i<pSizeSubV[MyRank];i++)
      	LocalMax = LocalMax > pSubVector[i] ? LocalMax : pSubVector[i];
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVI(stdout,pSubVector,pSizeSubV[MyRank],"Subvector Despues:");
     #endif
     
     MPI_Gatherv(pSubVector, pSizeSubV[MyRank], MPI_INT,
                 pVector,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(&LocalMax, &Max, 1, MPI_INT, MPI_MAX, Root, MPI_COMM_WORLD);
    }
 
  if (MyRank==Root)
    {
     pVector  = (int *) GetMem(Rows,sizeof(int),"Vector");
     InitVectorInt (pVector,pSizeSubV[Root]);
     
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVI(stdout,pVector,pSizeSubV[MyRank],"Subvector Antes Root:");
     #endif
     
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[Root];i++)
      	pVector[i]=pVector[i-pPosSubV[Root]]-x;
      	
     int Max=pVector[0];
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[Root];i++)
      	Max = Max > pVector[i] ? Max : pVector[i];
         
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVI(stdout,pVector,pSizeSubV[MyRank],"Subvector Despues Root:");
     #endif
     
     MPI_Gatherv(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL,
                 pVector,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(MPI_IN_PLACE, &Max, 1, MPI_INT, MPI_MAX, Root, MPI_COMM_WORLD);
     
     printf("Max=%d.\n",Max);
    }

 free((void *)pSubVector);
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 
  if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintVI(stdout,pVector,Rows,"Vector:");
     #endif
     free((void *)pVector);
    }


 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
