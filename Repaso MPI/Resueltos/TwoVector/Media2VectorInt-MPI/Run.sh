#!/bin/bash

make Media2VectorInt
echo "-----------------------------------------"
echo "Media2VectorInt serie"
echo "-----------------------------------------"
time ./Media2VectorInt -r 10

make Media2VectorInt-MPI
echo "-----------------------------------------"
echo "Media2VectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Media2VectorInt-MPI -r 10


#make Mult2VectorInt-MPI-InPlace
#echo "-----------------------------------------"
#echo "Mult2VectorInt MPI-InPlace"
#echo "-----------------------------------------"
#time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Mult2VectorInt-MPI-InPlace -r 10
