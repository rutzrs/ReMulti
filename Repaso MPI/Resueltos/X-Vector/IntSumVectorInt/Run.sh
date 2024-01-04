#!/bin/bash

make IntSumVectorInt

echo "-----------------------------------------"
echo "IntSumVectorInt serie"
echo "-----------------------------------------"
time ./IntSumVectorInt -r 20

make IntSumVectorInt-MPI

echo "-----------------------------------------"
echo "IntSumVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./IntSumVectorInt-MPI -r 10
