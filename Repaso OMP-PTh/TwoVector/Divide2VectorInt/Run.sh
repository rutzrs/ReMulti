#!/bin/bash

make Add2VectorInt

echo "-----------------------------------------"
echo "Add2VectorInt serie"
echo "-----------------------------------------"
time ./Add2VectorInt -r 10

make Add2VectorInt-OMP

export OMP_NUM_THREADS=4

echo "-----------------------------------------"
echo "SumMatrixInt-OMP"
echo "-----------------------------------------"
time ./Add2VectorInt-OMP -r 6 -c 7

make Add2VectorInt-PTh

echo "-----------------------------------------"
echo "SumMatrixInt-PTh"
echo "-----------------------------------------"
time ./Add2VectorInt-PTh -r 6 -c 7 -nt 4
