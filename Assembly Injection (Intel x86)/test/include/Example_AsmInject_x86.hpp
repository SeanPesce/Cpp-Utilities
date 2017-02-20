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

#ifdef _MSC_VER
// Using a Microsoft compiler
void __declspec(naked) __stdcall asmCodeExample();
#else
// Non-Microsoft compiler
void asmCodeExample();
#endif // _MSC_VER


#endif // EXAMPLE_ASM_INJECT_X86_HPP