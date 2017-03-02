# Created by Sean P

all: example_program

run: example_program
	./bin/example

example_program: compile_asm_inject_x64_funcs
	mkdir -p bin
	g++ -std=c++11 -masm=intel -I include -I ../include src/Example_AsmInject_x64.cpp build/AsmInject_x64.o -o bin/example

example_raw_asm:
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -I include -I ../include src/Example_AsmInject_x64.cpp -o asm/example.s
	g++ -std=c++11 -masm=intel -S -I ../include ../src/AsmInject_x64.cpp -o asm/AsmInject_x64.s

compile_asm_inject_x64_funcs:
	mkdir -p build
	g++ -std=c++11 -masm=intel -c -I ../include ../src/AsmInject_x64.cpp -o build/AsmInject_x64.o

clean:
	rm -rf build bin asm


all_ext: example_program_ext

run_ext: example_program_ext
	./bin/example

example_program_ext: compile_asm_inject_x64_funcs_ext
	mkdir -p bin
	g++ -std=c++11 -masm=intel -c -I include -I ../include src/Example_AsmInject_x64.cpp -o build/Example_AsmInject_x64.o
	g++ build/Example_AsmInject_x64.o build/AsmInject_x64.o build/AsmInject_x64_AsmFuncs.o -o bin/example

example_raw_asm_ext: get_premade_asm_funcs
	g++ -std=c++11 -masm=intel -S -I include -I ../include src/Example_AsmInject_x64.cpp -o asm/example.s
	g++ -std=c++11 -masm=intel -S -I ../include ../src/AsmInject_x64.cpp -o asm/AsmInject_x64.s

compile_asm_inject_x64_funcs_ext: compile_premade_asm_funcs
	g++ -std=c++11 -masm=intel -c -I ../include ../src/AsmInject_x64.cpp -o build/AsmInject_x64.o

compile_premade_asm_funcs:
	mkdir -p build
	as ../src/asm/AsmInject_x64.s -o build/AsmInject_x64_AsmFuncs.o

get_premade_asm_funcs:
	mkdir -p asm
	cp ../src/asm/AsmInject_x64.s asm/AsmInject_x64_AsmFuncs.s

