#!/bin/sh

echo "compile main"
g++ -c -o main.o main.cpp

echo "compile dsp"
g++ -c -o dsp.o source/dsp.cpp

echo "compile utils"
g++ -c -o utils.o source/utils.cpp

echo "create executable"
g++ -o main main.o dsp.o utils.o

echo "run"
echo "-------------------------------\n"
./main