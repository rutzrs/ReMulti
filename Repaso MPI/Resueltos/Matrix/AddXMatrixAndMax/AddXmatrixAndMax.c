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
 exit(0);
}

/*---------------------------------------------------------------------------*/
//Init values of a matrix of size Rows.
void InitMatrixInt(int *pMatrix, const int Size)
{ 
 for (int i=0;i<Size;i++)
     pMatrix[i]=lrand48()%10;
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
 int * pMatrix, Max, x;

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

 printf("Rows=%d. Cols=%d\n", Rows, Cols);

 //Init seed of random number generator
 srand48(time(NULL));

 //Calloc de Getmen put data to zeroes
 pMatrix  = (int *) GetMem(Rows*Cols,sizeof(int),"Main:Matrix");
 
 //Init values of Matrix
 InitMatrixInt (pMatrix,Rows*Cols);
 
 //Init x
 x=lrand48()%9+1;  //+1 to avoid x=0.
 printf("x=%d.\n",x);

 PrintMatrixInt(pMatrix,Rows*Cols,Cols,"Matrix");
 
 AddXmatrixAndMax(pMatrix,Rows*Cols,x,&Max);
 
 PrintMatrixInt(pMatrix,Rows*Cols,Cols,"Matrix+x");
 
 printf("Max=%d.\n",Max);

 free((void *)pMatrix);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
