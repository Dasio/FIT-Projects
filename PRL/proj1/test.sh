#!/bin/bash
if [ $# -lt 1 ];then 
    numbers=10;
else
    numbers=$1;
fi;

procs=$((numbers+1))

( dd if=/dev/urandom bs=1 count=$numbers of=numbers ) > /dev/null 2>&1

mpic++ -std=c++11 -o es es.cpp
mpirun -np $procs es

rm -f es numbers
