#!/bin/bash

make IntDivVectorInt

echo "-----------------------------------------"
echo "IntDivVectorInt serie"
echo "-----------------------------------------"
time ./IntDivVectorInt -r 20

make IntDivVectorInt-MPI

echo "-----------------------------------------"
echo "IntDivVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./IntDivVectorInt-MPI -r 10
