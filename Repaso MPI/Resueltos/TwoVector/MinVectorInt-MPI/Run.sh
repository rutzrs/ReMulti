#!/bin/bash

make MinVectorInt

echo "-----------------------------------------"
echo "MinVectorInt serie"
echo "-----------------------------------------"
time ./MinVectorInt -r 10

make MinVectorInt-MPI

echo "-----------------------------------------"
echo "MinVectorInt MPI"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MinVectorInt-MPI -r 10

make MinVectorInt-MPI-InPlace

echo "-----------------------------------------"
echo "MinVectorInt MPI-InPlace"
echo "-----------------------------------------"
time mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e ./MinVectorInt-MPI-InPlace -r 10

