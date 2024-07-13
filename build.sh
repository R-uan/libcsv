#!/bin/sh

apk update && \ 
    apk add --no-cache build-base

gcc -c -Wall -Werror -fPIC libcsv.c -o libcsv.o

gcc -shared -o libcsv.so libcsv.o

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

gcc -o main main.c -L. -lcsv

