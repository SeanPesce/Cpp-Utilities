// Made by Sean Pesce

#include "Example_AsmInject_x86.hpp"

// Depending on the compiler, the injection point could vary:
#ifdef _MSC_VER
    #define MAIN_OFFSET 211 // @todo: determine whether this is correct in Visual Studio
#else
    #define MAIN_OFFSET 211
#endif // _MSC_VER

int main()
{
    std::cout << "Press enter to begin.";
    std::string garbageString;
    // Wait for user input, allowing time to use a disassembler to determine injection information:
    getline(std::cin, garbageString);
    // Doesn't matter what the user inputs, we aren't using it.
    garbageString[0] = '\0'; // This statement helps avoid compiler errors

    for(int i = 0; i < 10; i++)
    {
        std::cout << "Value = " << NUMBER << std::endl; // Print NUMBER

        // Decrease NUMBER by 2:
        NUMBER -= 2;
        // Increase NUMBER by 3:
        NUMBER += 3;
        // Decrement NUMBER:
        NUMBER--; // Memory location: &main + MAIN_OFFSET
        // Note that NUMBER doesn't change between loop iterations, unless injection is used to remove or alter the above statement(s)

        // Halfway through the program, inject ASM to remove the decrement statement:
        if(i == 4)
        {
            // Unprotect the memory:
            #ifdef _WIN32
                uint32_t oldProtection;
                SET_MEM_PROTECTION(getMemPage((uint8_t*)&main + MAIN_OFFSET), 1, PAGE_EXECUTE_READWRITE, &oldProtection);
            #else
                SET_MEM_PROTECTION(getMemPage((uint8_t*)&main + MAIN_OFFSET), 1, PROT_READ|PROT_WRITE|PROT_EXEC, NULL);
            #endif  // _WIN32
            
            // Write the jump to the assembly function:
            injectASM(((uint8_t*)&main + MAIN_OFFSET), &JUMPBACK_ADDRESS, 3, (void*)&asmCodeExample);
            
            std::cout << "Injecting assembly at " << (int*)((uint8_t*)&main + MAIN_OFFSET) <<
                        " with return address " << (int*)JUMPBACK_ADDRESS << "..." << std::endl;
        }
        // After injection, NUMBER will change in future iterations of the loop.
    }
    return 0;
}

#ifdef _MSC_VER
// Microsoft compiler; use Visual Studio in-line ASM:
void __declspec(naked) __stdcall asmCodeExample()
{
	__asm
	{
		JMP JUMPBACK_ADDRESS
	}
}
#else
// Non-Microsoft compiler; use GCC in-line ASM:
void asmCodeExample()
{
    // The first ASM instruction is +3 from &asmCodeExample when using GCC/G++
    __asm__ volatile
    (
        "JMP [JUMPBACK_ADDRESS]" // To reference an outside variable, might need to prefix it with "_"
    );
}
#endif // _MSC_VER


