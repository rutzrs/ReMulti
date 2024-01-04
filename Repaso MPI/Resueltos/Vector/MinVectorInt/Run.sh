#!/bin/bash

make MinVectorInt

echo "-----------------------------------------"
echo "MinVectorInt serie"
echo "-----------------------------------------"
time ./MinVectorInt -r 20

make MinVectorInt-MPI

echo "-----------------------------------------"
echo "MinVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./MinVectorInt-MPI -r 10
