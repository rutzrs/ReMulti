#!/bin/bash

make MediaMatrixInt

echo "-----------------------------------------"
echo "MediaMatrixInt serie"
echo "-----------------------------------------"
time ./MediaMatrixInt -r 3 -c 3

make MediaMatrixInt-OMP

export OMP_NUM_THREADS=4

echo "-----------------------------------------"
echo "SumMatrixInt-OMP"
echo "-----------------------------------------"
time ./MediaMatrixInt-OMP -r 6 -c 7

make MediaMatrixInt-PTh

echo "-----------------------------------------"
echo "SumMatrixInt-PTh"
echo "-----------------------------------------"
time ./MediaMatrixInt-PTh -r 4 -c 2 -nt 4
