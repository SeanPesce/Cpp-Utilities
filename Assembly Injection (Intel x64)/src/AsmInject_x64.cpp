// Made by Sean Pesce

/* @TODO:   "Safe" functions have fundamental flaw: they're singletons and can't be used multiple
 *          times in the same instance with overwriting each other. Fix this (heap/objects?)
 */

#include "AsmInject_x64.hpp"


#ifndef _MSC_VER
    // Pointers for the JMP values from ASM trampoline function (GCC method):
    uint64_t    *TRAMPOLINE_JMP_TO_PTR = (uint64_t*)((uint8_t*)&TRAMPOLINE_FUNC + 25),  // To the user's code
                *TRAMPOLINE_RET_TO_PTR = (uint64_t*)((uint8_t*)&TRAMPOLINE_FUNC + 15);  // To the original code after the injection point
#endif // _MSC_VER



/* Injects a JMP r/m64 instruction at the given address.
 *  Notes:
 *      Space required: 14 bytes
 *      Trampoline function? Yes
 *      Registers preserved? Yes
 */
void injectJmp_14B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode) // @TODO: complete/test this function for Windows
{
    // Write the injected bytecode and store the final write offset (relative to the injection point):
    int popRaxOffset = writeBytecode_14B(injectionAddr, nopCount, (void*)TRAMPOLINE_FUNC); // The returned offset is also the offset of the POP %rax instruction

    setTrampolineJmpValues(asmCode, ((uint8_t*)injectionAddr + popRaxOffset), returnJmpAddr);
}



/* Injects a JMP r/m64 instruction at the given address.
 *  Notes:
 *      Space required: 14 bytes
 *      Trampoline function? No
 *      Registers preserved? No
 *          User must start their code with POP %rax and end their code with PUSH %rax/MOVABS %rax, returnJmpAddr/JMP %rax
 */
void injectJmp_14B_Unsafe(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode)
{
    // Write the injected bytecode and store the final write offset (relative to the injection point):
    int popRaxOffset = writeBytecode_14B(injectionAddr, nopCount, asmCode); // The returned offset is also the offset of the POP %rax instruction

    // Direct the user's return JMP to the POP %rax instruction:
    *(uint64_t*)returnJmpAddr = (uint64_t)((uint8_t*)injectionAddr + popRaxOffset);
}



/* Injects a JMP rel8 instruction at the given address.
 *  Notes:
 *      Space required: 2 bytes
 *      Local space required: 16 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the distant trampoline function
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel8                // 2 bytes; rel8 is the offset to the address of the first original instruction after the injection point
 *      Trampoline function? Yes, 2 (local and distant)
 *      Registers preserved? Yes
 */
void injectJmp_2B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                  void *localTrampoline, int trampNopCount)
{
    // Write the injected JMP rel8 instruction and local trampoline:
    writeBytecode_2B(injectionAddr, nopCount, localTrampoline, trampNopCount, (void*)TRAMPOLINE_FUNC);

    // Set the values of the pointers for the distant code cave and the user's final returning JMP:
    setTrampolineJmpValues(asmCode,
                           (uint8_t*)localTrampoline + PUSH_RAX_INSTR_LENGTH
                                                     + MOVABS_INSTR_LENGTH
                                                     + JMP_ABS_RAX_INSTR_LENGTH,
                           returnJmpAddr);
}



/* Injects a JMP rel8 instruction at the given address.
 *  Notes:
 *      Space required: 2 bytes
 *      Local space required: 16 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the distant trampoline function
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel8                // 2 bytes; rel8 is the offset to the address of the first original instruction after the injection point
 *      Trampoline function? Yes, 1 (local)
 *      Registers preserved? No
 *              User must start their code with POP %rax and end their code with PUSH %rax (before the final JMP instruction)
 */
void injectJmp_2B_Unsafe(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                  void *localTrampoline, int trampNopCount)
{
    // Write the injected JMP rel8 instruction and local trampoline:
    writeBytecode_2B(injectionAddr, nopCount, localTrampoline, trampNopCount, asmCode);

    // Obtain the offset of the POP %rax instruction in the local trampoline function:
    int popRaxOffset = PUSH_RAX_INSTR_LENGTH + MOVABS_INSTR_LENGTH + JMP_ABS_RAX_INSTR_LENGTH;

    // Direct the user's return JMP to the POP %rax instruction in the local trampoline function:
    *(uint64_t*)returnJmpAddr = (uint64_t)((uint8_t*)injectionAddr + popRaxOffset);
}



// Calculates the offset between a JMP rel instruction and some address:
int64_t calculateJmpOffset(void *fromAddress, void *toAddress, int jmpInstrLength)
{
    return ((uint64_t)toAddress - (uint64_t)fromAddress - jmpInstrLength);
}



// Set the memory protection permissions for a given section of memory:
int SET_MEM_PROTECTION(void *address, size_t size, uint32_t newProtection, uint32_t *oldProtection)
{
    #ifdef _WIN32
    
        // Windows (use VirtualProtect)
        return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)oldProtection);
    
    #else // _WIN32 not defined

        // Unix (use mprotect)
        oldProtection = NULL; // This line is to avoid compiler errors; oldProtection is not used on Unix systems @todo: implement oldProtection for Unix
        return mprotect(getMemPage(address), size, (int)newProtection); // getMemPage is called to obtain a page-aligned address

    #endif // _WIN32
}



// Obtain the starting address of the page of memory that contains the given memory address:
void *getMemPage(void *memAddress)
{
    // Obtain the system memory page size:
    #ifdef _WIN32 // Windows:
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        uint64_t pageSize = (uint64_t)sysInfo.dwPageSize;
    #else // Unix:
        uint64_t pageSize = (uint64_t)sysconf(_SC_PAGESIZE);
    #endif // _WIN32
    
    // Calculate the base address of the page and return it:
    return (void*)((uint64_t)memAddress - ((uint64_t)memAddress % pageSize));
}



#ifndef _MSC_VER
// Non-Microsoft compiler; use GCC in-line ASM:
void TRAMPOLINE_FUNC()
{
    // The first ASM instruction is +4 from &asmCodeExample when using GCC/G++
    __asm__ volatile
    (
        "pop %rax\n"
        "jmp QWORD PTR [TRAMPOLINE_JMP_TO_LBL]\n"
        "push %rax\n"
        "movabs %rax, 0x1111111111111111\n" // This imm64 value will be overwritten with the return address
        "jmp %rax\n"
        /* The following 8 NOP instructions will create a buffer for *TRAMPOLINE_JMP_TO_PTR to
         *  be stored locally, allowing for a known constant JMP operand size:       */
        "TRAMPOLINE_JMP_TO_LBL:\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
    );
}
#endif // _MSC_VER


// Helper function that writes the bytecode for 14-byte JMP injections and overwrites remaining
//     garbage bytecode with the specified number of NOP instructions.      
int writeBytecode_14B(void *injectionAddr, int nopCount, void *jmpTo)
{
    int writeOffset = 0; // Keep track of the offset to write to (relative to the injection point)

    // Begin writing at the injection point...
    //
    // PUSH %rax:
    *(uint8_t*)injectionAddr = PUSH_RAX_INSTR;
    writeOffset += PUSH_RAX_INSTR_LENGTH;
    //
    //
    // MOVABS %rax, imm64:
    *(uint16_t*)((uint8_t*)injectionAddr + writeOffset) = *(uint16_t*)MOVABS_RAX_INSTR_OPCODE; // Opcode of MOVABS %rax, imm64
    writeOffset += MOVABS_OPCODE_LENGTH;
    #ifdef _MSC_VER
        // Using a Microsoft compiler; jump straight to the intermediate trampoline code cave:
        *(uint64_t*)((uint8_t*)injectionAddr + writeOffset) = (uint64_t)jmpTo; // Operand of MOVABS %rax, imm64
    #else
        // Using non-MS compiler; GCC in-line ASM starts +4 bytes from asmCode:
        *(uint64_t*)((uint8_t*)injectionAddr + writeOffset) = (uint64_t)((uint8_t*)jmpTo+4); // Operand of MOVABS %rax, imm64
    #endif // _MSC_VER
    writeOffset += MOVABS_OPERAND_LENGTH;
    //
    //
    // JMP %rax:
    *(uint16_t*)((uint8_t*)injectionAddr + writeOffset) = *(uint16_t*)JMP_ABS_RAX_INSTR;
    writeOffset += JMP_ABS_RAX_INSTR_LENGTH;
    //
    //
    // POP %rax:
    *(uint8_t*)((uint8_t*)injectionAddr + writeOffset) = POP_RAX_INSTR;

    // Erase trailing garbage bytes from overwritten instruction(s):
    memset((void*)((uint8_t*)injectionAddr + writeOffset + POP_RAX_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);

    return writeOffset;
}


// Helper function that writes bytecode for 2-byte JMP injections and the
//  local trampoline functions they utilize.
void writeBytecode_2B(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo)
{
    // Write the injected JMP rel8 instruction:
    *(uint8_t*)injectionAddr = JMP_REL8_INSTR_OPCODE; // JMP rel8 opcode
    //
    *((uint8_t*)injectionAddr+1) = (int8_t)calculateJmpOffset(injectionAddr, localTrampoline, JMP_REL8_INSTR_LENGTH); // JMP rel8 operand
    
    // Erase trailing garbage bytes from overwritten instruction(s) at injection point:
    memset((void*)((uint8_t*)injectionAddr + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);

    // Create the local trampoline function:
    int retJmpOffset = writeBytecode_14B(localTrampoline, trampNopCount+JMP_REL8_INSTR_LENGTH, (void*)jmpTo); // Extra NOPs because some NOPs will be overwritten with the "JMP rel8" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel8 instruction (relative to localTrampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    *((uint8_t*)localTrampoline + retJmpOffset) = JMP_REL8_INSTR_OPCODE; // JMP rel8 opcode
    //
    *((uint8_t*)localTrampoline + retJmpOffset+1) = (int8_t)calculateJmpOffset(
                                                                (uint8_t*)localTrampoline+retJmpOffset,
                                                                (uint8_t*)injectionAddr+JMP_REL8_INSTR_LENGTH,
                                                                JMP_REL8_INSTR_LENGTH); // JMP rel8 operand

}



// Helper function that writes the appropriate values to the JMP pointers for the trampoline
//      function and user's final returning JMP instruction:
void setTrampolineJmpValues(void *trampJmpTo, void *trampRetTo, void *userRetTo)
{
    // Write the address for the JMP pointer from the intermediate ASM trampoline function to 
    //   the instruction after the injected JMP:                        
    #ifndef _MSC_VER
        // If on GCC, make ASM trampoline function writable:
        SET_MEM_PROTECTION((void*)&TRAMPOLINE_FUNC, 1, MEM_PROTECT_RWX, NULL);
    #endif // _MSC_VER
    *TRAMPOLINE_RET_TO_PTR = (uint64_t)trampRetTo;
    
    // Write the address for the JMP pointer from the intermediate ASM trampoline function to the user's ASM function:
    #ifdef _MSC_VER
        // Using a Microsoft compiler; jump straight to the user's assembly code:
        *TRAMPOLINE_JMP_TO_PTR = (uint64_t)trampJmpTo;
    #else
        // Using non-MS compiler; GCC ASM starts +4 bytes from asmCode:
        *TRAMPOLINE_JMP_TO_PTR = (uint64_t)((uint8_t*)trampJmpTo+4);
    #endif // _MSC_VER

    // Direct the user's return JMP to the intermediate trampoline function's return JMP:
    *(uint64_t*)userRetTo = (uint64_t)((uint8_t*)TRAMPOLINE_FUNC + TRAMPOLINE_JMPBACK_INSTR_OFFSET);
}