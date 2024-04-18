all: main

repair_compress.o: repair_compress.cpp repair_compress.h
	g++ -std=c++20 -O3 -c repair_compress.cpp

main: main.cpp repair_compress.o utils.h
	g++ -std=c++20 -O3 -o main main.cpp repair_compress.o

clean:
	rm -f main repair_compress.o