// Made by Sean Pesce

#include "Example_AsmInject_x64.hpp"

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

        // Decrease NUMBER by 2:
        NUMBER -= 2;

        std::cout << "Value = " << NUMBER << std::endl; // Print NUMBER

        // Increase NUMBER by 3:
        NUMBER += 3;

        // This conditional will never execute; it's here to stop compiler optimizations that might break the program:
        if(NUMBER == 1){ std::cout << "Value = " << NUMBER << std::endl; }

        // Decrement NUMBER:
        NUMBER--; // Memory location: &main + MAIN_OFFSET
        // Note that NUMBER doesn't change between loop iterations, unless injection is used to remove or alter the above statement(s)

        // Halfway through the program, inject ASM to remove the decrement statement:
        if(i == 4)
        {
            // Unprotect the memory:
            uint32_t oldProtection; // oldProtection is not utilized in Unix (might add this functionality later)
            SET_MEM_PROTECTION((uint8_t*)&main + MAIN_OFFSET, MEM_PROTECT_SIZE, MEM_PROTECT_RWX, &oldProtection);
            
            // Write the jump to the assembly function:
            injectJmp_14B(((uint8_t*)&main + MAIN_OFFSET), &JUMPBACK_ADDRESS, NOP_COUNT, (void*)&asmCodeExample);
            
            std::cout << "Injecting assembly at " << (int*)((uint8_t*)&main + MAIN_OFFSET) <<
                        " with return address " << (uint64_t*)JUMPBACK_ADDRESS << "..." << std::endl;
        }
        // After injection, NUMBER will change in future iterations of the loop.
    }
    return 0;
}


#ifndef _MSC_VER
// Non-Microsoft compiler; use GCC in-line ASM:
void asmCodeExample()
{
    // The first ASM instruction is +4 from a function's base address when using GCC/G++ 64-bit
    __asm__ volatile
    (
        "POP %rax\n"

        // User code would normally go here
        
        "PUSH %rax\n"
        "MOVABS %rax, [JUMPBACK_ADDRESS]\n" // To reference an outside variable, might need to prefix it with "_"
        "JMP %rax\n"
    );
}
#endif // _MSC_VER

