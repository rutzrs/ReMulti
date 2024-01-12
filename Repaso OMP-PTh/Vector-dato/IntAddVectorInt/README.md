# Examen de MPI.  Multiprocesadores ING-INF UAL

# A rellenar por el alumno

## Nombre y Apellidos: <....>
## Grupo de trabajo:  <....>

## Descripción de la arquitectura utilizada: 
usar $ lscpu y $ free -h.

  * Microprocesador:
  * Número de núcleos:
  * Cantidad de subprocesos por núcleo:
  * Tiene hyperthreading (SMT) activado en BIOS:
  * RAM:
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 



## Instrucciones:

  * Se adjunta la versión serie donde con TIMES=1 se pueden obtener los tiempos secuenciales de las partes de código serie  que se pueden paralelizar. 
  
  * Con PRINT=1 se imprime por pantalla, para ver si los resultados con correctos. Para medir tiempos PRINT=0.
  
  * El alumno debe realizar la versión MPÎ.

  * Los parámetros de los programas se encuentran en el Run.sh, pero el número de iteraciones hay que establecerlo para que el secuencial dure al menos unos segundos. Se debén añadir el comando para la ejecución MPI. 

  * La memoria se asigna de forma dinámica mediante las rutinas en getmem.h.
  
  * Los parámetros de entrada se obtienen mediante las rutinas en arghand.h.
  
  * Los vectores y matrices se imprimen mediante las rutinas en utils.h.
  
  * Se debe adjuntar un solo makefile que permita compilar los tres programas y con make clean limpie lo generado.
  
  * Se puede paralelizar la inicialización del (los) array(s) de datos aleatorios y la operación sobre  el (los) array(s).
  
  * Es obligatorio paralelizar al menos la operación sobre el (los) array(s). Se valorará más si se paraleliza además la inicialización aleatoria de el (los) array(s).
  
  * Se obtiene más nota si se realizan comunicaciones colectivas y si estas son IN\_PLACE.

  * Usa un tamaño de iteraciones pequeño para ver que se inicializa bien y se realiza bien la operación.
  
  * Cuando se miden tiempos del programa completo hay que compilar sin -g ni -pg y las variables PRINT y TIMES deben ser 0.

## Speed-up teórico del algoritmo secuencial.

1. ¿Qué número de iteraciones (NIter) se ha elegido para que el programa secuencial dure al menos unos segundos?

2. Para el programa secuencial, que se proporciona, rellena las siguientes tablas. Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.


 **Si se paraleliza solo la operación** 

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T.secuencial |                |
|T.suma       |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |

**Si se paraleliza la inicialización de el (los) array(s) y la operación**

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T.secuencial |                |
|T.inic.      |                |
|T.suma       |                | 
|T.CsPar      |                |
|SpA(2)       |                |
|SpA(4)       |                |



donde

T.secuencial:
El wall-clock time (tiempo total) del programa secuencial. Parte real del $time < programa > ... 

T.inic.:
El tiempo de inicializar el vector.

T.suma: 
El tiempo de realizar la suma del vector.

T.CsPar:
El tiempo del código secuencial que será paralelizado. 

SpA(p):
El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.


## Resultados del algoritmo paralelo con MPI

3. ¿Qué parte(s) has paralelizado de la versión secuencial?

4. ¿Has usado colectivas?

5. ¿Has usado IN_PLACE?

6. Rellena la siguiente tabla para la ejecución paralela realizada.
Se puede usar la hoja de cálculo Speed-up.ods para realizar los cálculos.
Se recuerda que no es necesario medir tiempos dentro del programa paralelo.

| Ejecución   | -r 500000000  | 
| ----------- | -------------- |
|T. par. 1 th |                |
|T. par. 2 th |                |
|T. par. 4 th |                |
|Sp(1)		  |                |
|Sp(2)		  |                |
|Sp(4)        |                |

Donde 

T. par. p th:
El tiempo total (parte real de la salida $time < programa > ...) del algoritmo paralelo con p hebras.

Sp(p):
Speed-up real con p tareas paralelas.


- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
