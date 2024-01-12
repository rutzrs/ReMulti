#!/bin/bash

make IntSubVectorInt

echo "-----------------------------------------"
echo "IntSubVectorInt serie"
echo "-----------------------------------------"
time ./IntSubVectorInt -r 20

make IntSubVectorInt-OMP

export OMP_NUM_THREADS=4

echo "-----------------------------------------"
echo "IntSubVectorInt-OMP"
echo "-----------------------------------------"
time ./IntSubVectorInt-OMP -r 6 -c 7

make IntSubVectorInt-PTh

echo "-----------------------------------------"
echo "IntSubVectorInt-PTh"
echo "-----------------------------------------"
time ./IntSubVectorInt-PTh -r 6 -c 7 -nt 4
