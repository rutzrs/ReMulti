#!/bin/bash

make MaxVectorInt

echo "-----------------------------------------"
echo "MaxVectorInt serie"
echo "-----------------------------------------"
time ./MaxVectorInt -r 10

make MaxVectorInt-MPI

echo "-----------------------------------------"
echo "MaxVectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MaxVectorInt-MPI -r 10

make MaxVectorInt-MPI-InPlace

echo "-----------------------------------------"
echo "MaxVectorInt MPI-InPlace"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MaxVectorInt-MPI-InPlace -r 10

