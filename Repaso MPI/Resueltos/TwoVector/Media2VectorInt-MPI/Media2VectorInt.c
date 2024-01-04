/*
Progam that generate two vectors (1 and 2) of random ints in [0,9] and
performs Vector3[i]=Vector1[i]+Vector2[i]
*/

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
//Init values of a vector of size Rows.
void InitVectorInt(int *pVector, const int Rows)
{
 for (int i=0;i<Rows;i++)
     pVector[i]=lrand48()%10;
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
//Print a vector of size Rows
void PrintMedia(int media)
{
 printf("-----------------------------------------\n");
 printf("La media es: \n");
     printf("%2d", media);
}

/*---------------------------------------------------------------------------*/
//Adds Vector1 and Vector2 in Vector3
int Media2VectorInt(int *pVector1, int *pVector2, int *pVector3, int media, const int Rows)
{
 media = 0;
 for (int i=0;i<Rows;i++){
     pVector3[i] = pVector1[i]+pVector2[i];
     media += pVector3[i];
   }
  return  media/Rows;
}

/*---------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
 int Rows;
 int * pVector1, * pVector2, * pVector3;

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

 //Calloc de Getmen put data to zeroes
 pVector1  = (int *) GetMem(Rows,sizeof(int),"Main:Vector1");
 pVector2  = (int *) GetMem(Rows,sizeof(int),"Main:Vector2");
 pVector3  = (int *) GetMem(Rows,sizeof(int),"Main:Vector3");

 //Init values of Vector1 and 2
 InitVectorInt (pVector1,Rows);
 InitVectorInt (pVector2,Rows);


 PrintVectorInt(pVector1,Rows,"Vector1");
 PrintVectorInt(pVector2,Rows,"Vector2");

 int media,calculoMedia;
 calculoMedia = Media2VectorInt(pVector1,pVector2,pVector3,media,Rows);
 PrintVectorInt(pVector3,Rows,"Vector3");
 PrintMedia(calculoMedia);

 free((void *)pVector3);
 free((void *)pVector2);
 free((void *)pVector1);

 return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
