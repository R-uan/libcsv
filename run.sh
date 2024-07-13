gcc -c -Wall -Werror -fPIC libcsv.c -o ./lib/libcsv.o && \
    gcc -shared -o ./lib/libcsv.so ./lib/libcsv.o

gcc -o ./out/main main.c -L./lib -lcsv

export LD_LIBRARY_PATH=./lib

./out/main
