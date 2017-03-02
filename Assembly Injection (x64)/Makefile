# Made by Sean P

all: compile_asm
	g++ -std=c++11 -masm=intel -c -I include src/AsmInject_x64.cpp -o build/AsmInject_x64.o

asm:
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -I include src/AsmInject_x64.cpp -o asm/AsmInject_x64.s
	cp src/asm/AsmInject_x64.s asm/AsmInject_x64_AsmFuncs.s

compile_asm:
	mkdir -p build
	as src/asm/AsmInject_x64.s -o build/AsmInject_x64_AsmFuncs.o

clean:
	rm -rf build bin asm

