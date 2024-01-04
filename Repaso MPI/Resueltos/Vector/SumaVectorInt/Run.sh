#!/bin/bash

make SumVectorInt

echo "-----------------------------------------"
echo "SumVectorInt serie"
echo "-----------------------------------------"
time ./SumVectorInt -r 10

make SumVectorInt-MPI

echo "-----------------------------------------"
echo "SumVectorInt MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./SumVectorInt-MPI -r 10


