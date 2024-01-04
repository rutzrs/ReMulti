#!/bin/bash

make AddXvectorAndMin

echo "-----------------------------------------"
echo "AddXvectorAndMin serie"
echo "-----------------------------------------"
./AddXvectorAndMin -r 10

make AddXvectorAndMin-PTh

echo "-----------------------------------------"
echo "AddXvectorAndMin-PTh"
echo "-----------------------------------------"
time ./AddXvectorAndMin-PTh -r 10 -nt 1

echo "-----------------------------------------"
echo "AddXvectorAndMin-PTh"
echo "-----------------------------------------"
time ./AddXvectorAndMin-PTh -r 10 -nt 2

echo "-----------------------------------------"
echo "AddXvectorAndMin-PTh"
echo "-----------------------------------------"
time ./AddXvectorAndMin-PTh -r 10 -nt 4

make AddXvectorAndMin-MPI

echo "-----------------------------------------"
echo "AddXvectorAndMin MPI"
echo "-----------------------------------------"
mpirun -np 1 xterm -fa 'Monospace' -fs 12 -hold -e  ./AddXvectorAndMin-MPI -r 10

echo "-----------------------------------------"
echo "AddXvectorAndMin MPI"
echo "-----------------------------------------"
mpirun -np 2 xterm -fa 'Monospace' -fs 12 -hold -e  ./AddXvectorAndMin-MPI -r 10

echo "-----------------------------------------"
echo "AddXvectorAndMin MPI"
echo "-----------------------------------------"
mpirun -np 4 xterm -fa 'Monospace' -fs 12 -hold -e  ./AddXvectorAndMin-MPI -r 10
