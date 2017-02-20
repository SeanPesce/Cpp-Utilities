
all: example_program_on_x64
	
example_program: compile_asm_inject_x86_funcs
	mkdir -p bin
	g++ -std=c++11 -masm=intel -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_program_on_x64: compile_asm_inject_x86_funcs_on_x64
	mkdir -p bin
	g++ -std=c++11 -masm=intel -m32 -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_asm: compile_asm_inject_x86_funcs
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

example_asm_on_x64: compile_asm_inject_x86_funcs_on_x64
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -m32 -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

compile_asm_inject_x86_funcs:
	# If compiling on a 64-bit system, compile with 'compile_asm_inject_x86_funcs_on_x64'
	mkdir -p build
	g++ -std=c++11 -masm=intel -c -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

compile_asm_inject_x86_funcs_on_x64:
	# If compiling on a 32-bit system, compile with 'compile_asm_inject_x86_funcs'
	# On a 64-bit system, make sure the following packages are installed: gcc-multilib g++-multilib
	mkdir -p build
	g++ -std=c++11 -masm=intel -m32 -c -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

example_program_dbg: compile_asm_inject_x86_funcs_dbg
	mkdir -p bin
	g++ -std=c++11 -masm=intel -DDEBUG -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_program_on_x64_dbg: compile_asm_inject_x86_funcs_on_x64_dbg
	mkdir -p bin
	g++ -std=c++11 -masm=intel -DDEBUG -m32 -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_asm_dbg: compile_asm_inject_x86_funcs_dbg
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -DDEBUG -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

example_asm_on_x64_dbg: compile_asm_inject_x86_funcs_on_x64_dbg
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -DDEBUG -m32 -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

compile_asm_inject_x86_funcs_dbg:
	# If compiling on a 64-bit system, compile with 'compile_asm_inject_x86_funcs_on_x64'
	mkdir -p build
	g++ -std=c++11 -masm=intel -c -DDEBUG -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

compile_asm_inject_x86_funcs_on_x64_dbg:
	# If compiling on a 32-bit system, compile with 'compile_asm_inject_x86_funcs'
	# On a 64-bit system, make sure the following packages are installed: gcc-multilib g++-multilib
	mkdir -p build
	g++ -std=c++11 -masm=intel -m32 -c -DDEBUG -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

clean:
	rm -rf build bin asm

