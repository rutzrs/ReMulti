#!/bin/bash

make MaxVectorInt

echo "-----------------------------------------"
echo "MaxVectorInt serie"
echo "-----------------------------------------"
time ./MaxVectorInt -r 20

make MaxVectorInt-MPI

echo "-----------------------------------------"
echo "MaxVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./MaxVectorInt-MPI -r 10
