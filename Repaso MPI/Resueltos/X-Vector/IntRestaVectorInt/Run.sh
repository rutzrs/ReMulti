#!/bin/bash

make IntRestaVectorInt

echo "-----------------------------------------"
echo "IntRestaVectorInt serie"
echo "-----------------------------------------"
time ./IntRestaVectorInt -r 20

make IntRestaVectorInt-MPI

echo "-----------------------------------------"
echo "IntRestaVector MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./IntRestaVectorInt-MPI -r 10
