/*
Programa que genera dos matrices (1 y 2) de enteros aleatorios en [0,9] y
realiza Matriz3[i]=Matriz1[i]+Matriz2[i] y usa Matriz3 para
encontrar el mínimo, máximo, suma y producto de sus valores
*/

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
//Inicializa valores de una matriz de tamaño Rows*Cols.
void InitMatrizInt(int *pMatriz, const int Size)
{ 
 for (int i=0;i<Size;i++)
     pMatriz[i]=lrand48()%10;
}

/*---------------------------------------------------------------------------*/
//Imprime una matriz de tamaño Rows*Cols.
void PrintMatrizInt(int *pMatriz, const int Size, const int Cols, char * String)
{
 printf("%s -----------------------------------------\n", String);
 for (int i=0;i<Size;i++)
{
     	printf("%2d,", pMatriz[i]);
	if((i+1)%Cols == 0)
		printf("\n");
}
 puts("");
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows,Cols;
 int max,sum,min;
 int sumTotal = 0;
 int minTotal = 100;
 int maxTotal = 0;
 long prod,prodTotal = 1;
 srand48(time(NULL));
 int random = lrand48()%10; //Numero aleatorio entre 0 y 9 que se le sumará a la matriz final
 int * pMatriz3   =NULL;
 int * pSubMatriz1=NULL, * pSubMatriz2=NULL, * pSubMatriz3=NULL;
 int NTasks, MyRank; 	//MPI number of task and Rank for current task.
 int *pPosSubV;			//Vector with position each task starts in Vectors.
 int *pSizeSubV;			//Vector with size of subvector.
 int Rem; 				//Remainder
 int AddPos;			//Adding positions.

 //MPI Initialization
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 
 if (MyRank==0)	//Task 0--
    {
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
	 //Inicializamos valores en root para hacer Broadcast a todas las tareas
	 sum = 0;
	 min = 100;
	 max = 0;
	 prod = 1;
	 
	 printf("Rows=%d.\n", Rows);
	 printf("Cols=%d.\n", Cols);
	 printf("Value x=%d.\n", random);
	 //Calloc de Getmen put data to zeroes
	 //pMatriz1 and 2 are not needed in task0
	 //They are generated in workers tasks
	 pMatriz3  = (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matriz3");

	}
	
 //All tasks 
 MPI_Bcast(&Rows,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&Cols,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&min,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&max,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&sum,1,MPI_INT,0,MPI_COMM_WORLD);
 MPI_Bcast(&prod,1,MPI_INT,0,MPI_COMM_WORLD);

 //Get mem and initialize DisplacV and pSizeSubV
 pPosSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:DisplacV");
 pSizeSubV = (int *)GetMem(NTasks,sizeof(int),"All tasks:pSizeSubV");
 
 Rem=Rows*Cols%NTasks; 
 AddPos=0;
 for (int i=0; i<NTasks; i++) 
     {
      pSizeSubV[i] = Rows*Cols/NTasks;
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
 
 pSubMatriz1  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "SubMatriz1");
 pSubMatriz2  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "pSubMatriz2");
 pSubMatriz3  = (int *) GetMem(pSizeSubV[MyRank],sizeof(int),
                               "pSubMatriz3");

 //Init seed of random number generator to the task rank
 //each sequence is local to each task becasue tasks do not share variables.
 //Semilla para el generador de números aleatorios que depende del rank de la tarea
 srand48(time(NULL)*MyRank);

 //All Init values of SubVector1 and 2
 InitMatrizInt (pSubMatriz1,pSizeSubV[MyRank]);
 InitMatrizInt (pSubMatriz2,pSizeSubV[MyRank]);
 
 #if (PRINT==1)
 printf("Task %d init vectors:\n", MyRank);
 PrintMatrizInt(pSubMatriz1,pSizeSubV[MyRank],Cols,"SubVector1:");
 PrintMatrizInt(pSubMatriz2,pSizeSubV[MyRank],Cols,"SubVector2:");
 #endif

 //Hacemos la suma de matrices y 
 //las operaciones de suma, producto, min y max
 for (int i=0;i<pSizeSubV[MyRank];i++){
     pSubMatriz3[i]=pSubMatriz1[i]+pSubMatriz2[i] + random;
	if(pSubMatriz3[i]<min)
		min = pSubMatriz3[i];
	if(pSubMatriz3[i]>max)
		max = pSubMatriz3[i];
	prod = prod*pSubMatriz3[i];
	sum += pSubMatriz3[i];
}

#if (PRINT==1)
 printf("Task %d init vectors:\n", MyRank);
 #endif

 MPI_Reduce(&prod,&prodTotal,1,MPI_INT,MPI_PROD,0,MPI_COMM_WORLD);
 MPI_Reduce(&sum,&sumTotal,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
 MPI_Reduce(&min,&minTotal,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
 MPI_Reduce(&max,&maxTotal,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

 MPI_Gatherv(pSubMatriz3, pSizeSubV[MyRank], MPI_INT,
             pMatriz3, pSizeSubV, pPosSubV, 
             MPI_INT,0, MPI_COMM_WORLD);
 
 free((void *)pSizeSubV);
 free((void *)pPosSubV);
 free((void *)pSubMatriz3);
 free((void *)pSubMatriz2);
 free((void *)pSubMatriz1);

 if (MyRank==0)
    {
     #if (PRINT==1)
     	PrintMatrizInt(pMatriz3,Rows*Cols,Cols,"Matriz3 + x");
	printf("Suma = %d\n", sumTotal);
	printf("Producto = %ld\n", prodTotal);
	printf("Min = %d\n", minTotal);
	printf("Max = %d\n", maxTotal);
     #endif
     free((void *)pMatriz3);
    }

 MPI_Finalize();
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
