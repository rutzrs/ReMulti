#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define DEBUG 1

/*---------------------------------------------------------------------------*/
void ParametersError()
{
 puts("Options are:");
 puts("\t[ -h To show this help ]");
 puts("\t  -r <n rows>			");
 exit(0);
}

/*---------------------------------------------------------------------------*/
void InitMatrixMin(int **pMatrix, const int Rows, const int Cols)
{
 for (int i=0;i<Rows;i++) {
	 for (int j = 0; j < Cols; j++) {
		 pMatrix[i][j]=lrand48()%10;
	}
 }
}

/*---------------------------------------------------------------------------*/
void PrintMatrixMin(int **pMatrix, const int Rows, const int Cols)
{
 puts("-----------------------------------------");
 for (int i=0;i<Rows;i++) {
	 printf("[");
	for (int j = 0; j < Cols; j++) {
		printf("%d,", pMatrix[i][j]);
	 }
	 puts("]");
 }
 puts("");
}

/*---------------------------------------------------------------------------*/
void MinMatrixMin(int **pMatrix, const int Rows, const int Cols, int *pMin)
{
 int Min=9999;
 
 for (int i=0;i<Rows;i++){
	 for (int j = 0; j < Cols; j++) {
		 if (pMatrix[i][j] < Min) Min = pMatrix[i][j];
	}
 }
 *pMin=Min;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int Cols;
 int ** pMatrix;
 int Min=9999;

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

 #if (DEBUG==1)
 printf("SumMatrixMin: Rows=%d Cols=%d.\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pMatrix  = (int **) GetMem2D(Rows, Cols,sizeof(int),"Main:Matrix");
 
 InitMatrixMin (pMatrix,Rows, Cols);
 PrintMatrixMin(pMatrix,Rows, Cols);
 MinMatrixMin  (pMatrix,Rows, Cols, &Min);
 
 printf("SumMatrixMin=%d.\n",Min);

 free((void **)pMatrix);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
