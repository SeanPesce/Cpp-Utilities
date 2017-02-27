// Made by Sean Pesce

#pragma once

#ifndef EXAMPLE_ASM_INJECT_X64_HPP
    #define EXAMPLE_ASM_INJECT_X64_HPP


#include <iostream> // cout, cin, getline
#include <string> // string
#include "AsmInject_x64.hpp" // Assembly Injection functions to be demonstrated with this program

int NUMBER = 12; // Will be printed as feedback to show when the injection occurs
uint64_t JUMPBACK_ADDRESS; // The address used to jump out of the assembly code cave to resume normal program logic

#ifdef _WIN32
    // Define Windows-specific values here
    #define MEM_PROTECT_SIZE 10
#else
    // Define Unix-specific values here
    #define MEM_PROTECT_SIZE 1
#endif // _WIN32


#ifdef _MSC_VER // Using a Microsoft compiler
    // Depending on the compiler, many things can vary:
    #define MAIN_OFFSET 190     // The injection point
    #define NOP_COUNT 41         // The type of instructions used (more importantly here, the number of leftover bytes from those overwritten instructions)
    void __stdcall asmCodeExample(); // Calling conventions for ASM code
#else
    // Non-Microsoft compiler
    #define MAIN_OFFSET 190
    #define NOP_COUNT 41
    void asmCodeExample();
#endif // _MSC_VER


#endif // EXAMPLE_ASM_INJECT_X64_HPP