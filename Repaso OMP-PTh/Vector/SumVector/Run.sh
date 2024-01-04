#!/bin/bash

make SumVectorInt

echo "-----------------------------------------"
echo "SumVectorInt serie"
echo "-----------------------------------------"
time ./SumVectorInt -r 10

make SumVectorInt-OMP

export OMP_NUM_THREADS=4 

echo "-----------------------------------------"
echo "SumVectorInt-OMP"
echo "-----------------------------------------"
time ./SumVectorInt -r 10

make SumVectorInt-PTh

echo "-----------------------------------------"
echo "SumVectorInt-PTh"
echo "-----------------------------------------"
time ./SumVectorInt-PTh -r 10 -nt 4
