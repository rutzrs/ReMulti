#!/bin/bash

make Resta2VectorInt
echo "-----------------------------------------"
echo "Resta2VectorInt serie"
echo "-----------------------------------------"
time ./Resta2VectorInt -r 10

'make Resta2VectorInt-MPI
echo "-----------------------------------------"
echo "Resta2VectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Resta2VectorInt-MPI -r 10
'

make Resta2VectorInt-MPI-InPlace
echo "-----------------------------------------"
echo "Resta2VectorInt MPI-InPlace"
echo "-----------------------------------------"
time mpirun -np 4 --oversubscribe xterm -fa 'Monospace' -fs 12 -hold -e ./Resta2VectorInt-MPI-InPlace -r 10
