#!/bin/bash

make RestaMatrixInt

echo "-----------------------------------------"
echo "RestaMatrixInt serie"
echo "-----------------------------------------"
time ./RestaMatrixInt -r 6 -c 7

make RestaMatrixInt-OMP

export OMP_NUM_THREADS=4

echo "-----------------------------------------"
echo "RestaMatrixInt-OMP"
echo "-----------------------------------------"
time ./RestaMatrixInt-OMP -r 6 -c 7

make RestaMatrixInt-PTh

echo "-----------------------------------------"
echo "RestaMatrixInt-PTh"
echo "-----------------------------------------"
time ./RestaMatrixInt-PTh -r 6 -c 7 -nt 4
