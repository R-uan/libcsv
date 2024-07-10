apk update && \ 
    apk add --no-cache build-base

g++ -c -Wall -Werror -fPIC libcsv.cpp -o libcsv.o
if [ $? -ne 0 ]; then
    echo "Compilation of libcsv.cpp failed."
    exit 1
fi

g++ -shared -o libcsv.so libcsv.o
if [ $? -ne 0 ]; then
    echo "Building shared library libcsv.so failed."
    exit 1
fi

echo "Shared library built successfully."
