// Made by Sean Pesce

#ifdef _MSC_VER
    // Using a Microsoft compiler
    #pragma once
#endif // _MSC_VER

#ifndef EXAMPLE_ASM_INJECT_X86_HPP
    #define EXAMPLE_ASM_INJECT_X86_HPP


#include <iostream> // cout, cin, getline
#include <string> // string
#include "AsmInject_x86.hpp" // Assembly Injection functions to be demonstrated with this program

int NUMBER = 10; // Will be printed as feedback to show when the injection occurs
uint32_t JUMPBACK_ADDRESS; // The address used to jump out of the assembly code cave to resume normal program logic

#ifdef _WIN32
    // Define Windows-specific values here
    #define MEM_PROTECT_SIZE 10
#else
    // Define Unix-specific values here
    #define MEM_PROTECT_SIZE 1
#endif // _WIN32


#ifdef _MSC_VER // Using a Microsoft compiler
    #define MAIN_OFFSET 211 // Depending on the compiler, the injection point could vary // @todo: determine whether this is correct in Visual Studio
    void __stdcall asmCodeExample();
#else
    // Non-Microsoft compiler
    #define MAIN_OFFSET 211
    void asmCodeExample();
#endif // _MSC_VER


#endif // EXAMPLE_ASM_INJECT_X86_HPP