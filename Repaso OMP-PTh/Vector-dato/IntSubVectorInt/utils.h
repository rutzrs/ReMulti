/******************************************************************************
			utils.h  - description
			----------------------
	begin		: Sep 2021
	copywirght	: (C) 2021 by L.G.Casado.

********************************************************************	
Purpose: implement routines that will be used by other modules. 
	OpenFile: Abrir un fichero con chequeo de error
	CloseFile: Cerrar un fichero
	PrintVI: Imprimir un vector de enteros
	PrintMI: Imprimir una matriz de enteros.

*******************************************************************************/
#ifndef __UTILS__
#define __UTILS__

/*---------------------------------------------------------------------------*/
FILE 	*OpenFile       (char *name, char const *mode);
void 	CloseFile       (FILE *f);
void 	PrintVI 	    (FILE * FOut, int * pVI, int NDim, char * String);
void 	PrintMI 	    (FILE * FOut, int ** ppMI, int Fil, int Col, 
						 char * String);

#endif /*__UTILS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
