#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
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
void InitVectorInt(int *pVector, const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%100;
}

/*---------------------------------------------------------------------------*/
void PrintVectorInt(int *pVector, const int Rows)
{
 puts("-----------------------------------------");
 for (int i=0;i<Rows;i++)
     printf("%d,", pVector[i]);
 puts("]");
}

/*---------------------------------------------------------------------------*/
void MediaVectorInt(int *pVector, const int Rows, int *pMedia)
{
 int Sum=0;
 
 for (int i=0;i<Rows;i++){
	Sum += pVector[i];
 }
 
 *pMedia=Sum/Rows;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector=NULL, * pSubVector=NULL;
 int Sum=0;
 int Media;
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
	    
	 printf("MediaVectorInt: Rows=%d.\n", Rows);

	 //Calloc de Getmen put data to zeroes
	 //pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 }
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,Root,MPI_COMM_WORLD);
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
 
 //Init seed of randum number generator
 srand48(MyRank);
 
  //Only workers needs psubvector3
 if (MyRank!=Root)
    {
     //Calloc de Getmen put data to zeroes
     pSubVector  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),"Main:SubVector");
     InitVectorInt (pSubVector,pSizeSubV[MyRank]);
     
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pSubVector,pSizeSubV[MyRank]);
     #endif
     
     int LocalSum=0;
     for (int i=0;i<pSizeSubV[MyRank];i++){
	LocalSum += pSubVector[i];
     }
              
     MPI_Gatherv(pSubVector, pSizeSubV[MyRank], MPI_INT,
                 pVector,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(&LocalSum, &Sum, 1, MPI_INT, MPI_SUM, Root, MPI_COMM_WORLD);
    }
    
  //Only workers needs psubvector3
 if (MyRank==Root)
    {
     //Calloc de Getmen put data to zeroes
     pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
     InitVectorInt (pVector,Rows);
     
     #if (PRINT==1)
     printf("Task %d  before gatherv:\n", MyRank);
     PrintVectorInt(pVector,pSizeSubV[MyRank]);
     #endif
     
     for (int i=pPosSubV[Root];i<pPosSubV[Root]+pSizeSubV[MyRank];i++){
	Sum += pVector[i-pPosSubV[Root]];
     }  
     
     MPI_Gatherv(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL,
                 pVector,    pSizeSubV, pPosSubV, MPI_INT,
                 Root, MPI_COMM_WORLD);
                 
     MPI_Reduce(MPI_IN_PLACE, &Sum, 1, MPI_INT, MPI_SUM, Root, MPI_COMM_WORLD);
    }
 //All deallocate subvectors
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubVector);

 if (MyRank==Root)
    {
     #if (PRINT==1)
     PrintVectorInt(pVector,Rows);
     #endif
     Media=(double)Sum/(double)Rows; 
     printf("SumVectorInt=%d\n",Sum);
     printf("MediaVectorInt=%d\n",Media);
     free((void *)pVector);
    }
 
 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
