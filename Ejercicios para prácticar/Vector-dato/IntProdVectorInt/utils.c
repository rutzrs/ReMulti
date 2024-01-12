#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/*---------------------------------------------------------------------------*/
FILE * OpenFile (char *name, char const *mode)
{
 FILE *f;

 f=fopen(name,mode);
 if (f==NULL)
    {
     fprintf(stderr,"Can not open the file %s\n",name);
     exit(1);
    }
 return f;
}

/*---------------------------------------------------------------------------*/
void CloseFile (FILE *f)
{
  if (fclose(f))
      fprintf(stderr,"Unable to close file.\n");
}

/*---------------------------------------------------------------------------*/
void PrintVI (FILE * FOut, int * pVI, int NDim, char * String)
{
 int i;

 fprintf(FOut,"%s : ", String);
 for (i=0;i<NDim;i++)
     fprintf(FOut,"%3d, ",(int)pVI[i]);
 fprintf(FOut,"\n");    
}

/*---------------------------------------------------------------------------*/
void PrintMI (FILE * FOut, int ** pMI, int Fil, int Col, char * String)
{
 int i;
 char cadena[255];
 printf("%s -----------------------------------------\n", String);
 for (i=0;i<Fil;i++)
     {
      sprintf(cadena,"fila %d",i);
      PrintVI(FOut,pMI[i],Fil,cadena);
     }
 fprintf(FOut,"\n");
}
