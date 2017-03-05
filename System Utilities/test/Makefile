# Made by Sean P

all: build_sysutils build_test

run: all
	./bin/test

build_test:
	mkdir -p bin
	g++ -std=c++11 -I ../include -I include -I ../../I-O/include build/SP_SysUtils.o ../../I-O/src/SP_Shell_IO.cpp src/SP_SysUtils_Test.cpp -o bin/test

build_sysutils:
	mkdir -p build
	g++ -std=c++11 -c -I ../include ../src/SP_SysUtils.cpp -o build/SP_SysUtils.o

clean:
	rm -rf bin build asm
