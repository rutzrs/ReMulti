#!/bin/bash

make all

echo "-----------------------------------------"
echo "SubXvectorAndMin serie"
echo "-----------------------------------------"
./SubXvectorAndMin -r 10

echo "-----------------------------------------"
echo "SubXvectorAndMin serie"
echo "-----------------------------------------"
./SubXvectorAndMin-OMP -r 10

echo "-----------------------------------------"
echo "SubXvectorAndMin serie"
echo "-----------------------------------------"
./SubXvectorAndMin-Pth -r 10


