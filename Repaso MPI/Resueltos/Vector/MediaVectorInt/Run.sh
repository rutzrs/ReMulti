#!/bin/bash

make IntMediaVectorInt

echo "-----------------------------------------"
echo "IntMediaVectorInt serie"
echo "-----------------------------------------"
time ./IntMediaVectorInt -r 20

make IntMediaVectorInt-MPI

echo "-----------------------------------------"
echo "IntMediaVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./IntMediaVectorInt-MPI -r 10
