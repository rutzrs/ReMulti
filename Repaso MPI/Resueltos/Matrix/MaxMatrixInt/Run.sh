#!/bin/bash

make MaxMatrixInt

echo "-----------------------------------------"
echo "MaxMatrixInt serie"
echo "-----------------------------------------"
time ./MaxMatrixInt -r 5 -c 5

make MaxMatrixInt-MPI
echo "-----------------------------------------"
echo "MaxMatrixInt MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./MaxMatrixInt-MPI -r 5 -c 5

