#!/bin/bash

make IntMultVectorInt

echo "-----------------------------------------"
echo "IntMultVectorInt serie"
echo "-----------------------------------------"
time ./IntMultVectorInt -r 20

make IntMultVectorInt-MPI

echo "-----------------------------------------"
echo "IntMultVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./IntMultVectorInt-MPI -r 10
