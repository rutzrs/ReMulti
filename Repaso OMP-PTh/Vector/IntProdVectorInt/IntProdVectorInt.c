#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

#define PRINT 1
#define TIMES 1

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
//Multiplies x to Vector
void IntProdVectorInt(int *pVector, const int Rows, const int dato)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=pVector[i]*dato;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int x;  //Integer to Add.
 int * pVector;


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

 printf("Rows=%d.\n", Rows);

 //Init seed of random number generator
 srand48(time(NULL));
 
 //Init x
 x=lrand48()%10;
 printf("x=%d.\n",x);

 //Calloc de Getmen put data to zeroes
 pVector  = (int *) GetMem(Rows,sizeof(int),"Main:Vector");
 
 //Init values of Vector
 #if (TIMES==1)
 struct timeval tv1,tv2,tv3,tv4; struct timezone tz;
 gettimeofday(&tv1, &tz);
 #endif
 InitVectorInt (pVector,Rows);
 #if (TIMES==1)
 gettimeofday(&tv2, &tz);
 #endif
 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector  ");
 #endif
 
 #if (TIMES==1)
 gettimeofday(&tv3, &tz);
 #endif
 IntProdVectorInt(pVector,Rows,x);
 #if (TIMES==1)
 gettimeofday(&tv4, &tz);
 double TInitV = (tv2.tv_sec- tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec)*1e-6;
 printf("Tiempo de inicializar el vector            = %12.4g sec\n", TInitV);
 double TOpV =  (tv4.tv_sec- tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec)*1e-6;
 printf("Tiempo de operación con el vector          = %12.4g sec\n", TOpV);
 printf("Tiempo de inicializar y op con vector      = %12.4g sec\n", 
         TInitV+TOpV);
 #endif
 
 #if (PRINT==1)
 PrintVI(stdout,pVector,Rows,"Vector*x");
 #endif
 
 free((void *)pVector);
 
 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
