#!/bin/bash

make Min2VectorInt

echo "-----------------------------------------"
echo "Min2VectorInt serie"
echo "-----------------------------------------"
time ./Min2VectorInt -r 10

make Min2VectorInt-OMP

export OMP_NUM_THREADS=4

echo "-----------------------------------------"
echo "Min2VectorInt-OMP"
echo "-----------------------------------------"
time ./Min2VectorInt -r 10

make Min2VectorInt-PTh

echo "-----------------------------------------"
echo "Min2VectorInt-PTh"
echo "-----------------------------------------"
time ./Min2VectorInt -r 10 -nt 4
