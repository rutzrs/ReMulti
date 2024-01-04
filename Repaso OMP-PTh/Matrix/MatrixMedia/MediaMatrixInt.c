#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
 puts("\t  -c <n cols>			");
 exit(0);
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
     pMatrix[i][j]=lrand48()%10;

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
void MediaMatrixInt(int **pMatrix, const int Rows, const int Cols, int *pSum,int * pmedia)
{
 int Sum=0;
 
 for (int i=0;i<Rows;i++)
    for (int j=0;j<Cols;j++)
     Sum+=pMatrix[i][j];

 *pSum=Sum;
 *pmedia=Sum/(Rows*Cols);
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int Cols;
 int **pMatrix;
 int Sum=0;
 int media = 0;

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
 printf("SumMatrixInt: Rows=%d\tCols=%d.\n", Rows, Cols);
 #endif

 //Init seed of randum number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pMatrix  = (int **) GetMem2D(Rows,Cols,sizeof(int),"Main:Vector");

 InitMatrixInt (pMatrix,Rows,Cols);

 #if (DEBUG==1)
 PrintMatrixInt(pMatrix,Rows,Cols);
 #endif

 MediaMatrixInt (pMatrix,Rows,Cols, &Sum,&media);

 printf("MediaMatrixInt=%d.\n",media);


 free((void **)pMatrix);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
