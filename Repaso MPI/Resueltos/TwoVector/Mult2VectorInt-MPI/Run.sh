#!/bin/bash

make Mult2VectorInt
echo "-----------------------------------------"
echo "Mult2VectorInt serie"
echo "-----------------------------------------"
time ./Mult2VectorInt -r 10

'make Mult2VectorInt-MPI
echo "-----------------------------------------"
echo "Mult2VectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Mult2VectorInt-MPI -r 10
'

make Mult2VectorInt-MPI-InPlace
echo "-----------------------------------------"
echo "Mult2VectorInt MPI-InPlace"
echo "-----------------------------------------"
time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Mult2VectorInt-MPI-InPlace -r 10
