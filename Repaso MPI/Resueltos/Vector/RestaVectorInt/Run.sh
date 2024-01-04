#!/bin/bash

make RestaVectorInt

echo "-----------------------------------------"
echo "RestaVectorInt serie"
echo "-----------------------------------------"
time ./RestaVectorInt -r 10

make RestaVectorInt-MPI

echo "-----------------------------------------"
echo "RestaVectorInt MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./RestaVectorInt-MPI -r 10


