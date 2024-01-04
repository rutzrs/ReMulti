#!/bin/bash

make ProdVectorInt

echo "-----------------------------------------"
echo "ProdVectorInt serie"
echo "-----------------------------------------"
time ./ProdVectorInt -r 10

make ProdVectorInt-MPI

echo "-----------------------------------------"
echo "ProdVectorInt MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./ProdVectorInt-MPI -r 10


