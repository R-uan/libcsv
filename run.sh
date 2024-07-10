g++ -c -Wall -Werror -fPIC libcsv.cpp -o ./lib/libcsv.o
if [ $? -ne 0 ]; then
    echo "Compilation of libcsv.cpp failed."
    exit 1
fi

g++ -shared -o ./lib/libcsv.so ./lib/libcsv.o
if [ $? -ne 0 ]; then
    echo "Building shared library libcsv.so failed."
    exit 1
fi

echo "Shared library built successfully."

export LD_LIBRARY_PATH=./lib
gcc -o ./out/main main.c -L./lib -lcsv
if [ $? -ne 0 ]; then
    echo "Compilation of main.c failed."
    exit 1
fi

echo "Main program compiled successfully."
echo ""
./out/main
if [ $? -ne 0 ]; then
    echo "Execution of main program failed."
    exit 1
fi
echo "Main program executed successfully."
