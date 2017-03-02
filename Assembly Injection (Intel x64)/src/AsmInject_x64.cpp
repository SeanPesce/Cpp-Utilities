// Made by Sean Pesce

#include "AsmInject_x64.hpp"



/* Injects code using a JMP r/m64 instruction at the given address.
 *  Notes:
 *      Space required: 14 bytes
 *      Registers preserved? No
 *          User must start their code with POP %rax and end their code with PUSH %rax/MOVABS %rax, returnJmpAddr/JMP %rax
 */
void injectJmp_14B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode)
{
    // Write the injected bytecode and store the final write offset (relative to the injection point):
    int popRaxOffset = writeJmpRax_14B(injectionAddr, asmCode, nopCount); // The returned offset is also the offset of the POP %rax instruction

    // Direct the user's return JMP to the POP %rax instruction:
    *(uint64_t*)returnJmpAddr = (uint64_t)((uint8_t*)injectionAddr + popRaxOffset);
}



/* Injects code using a JMP rel8 instruction at the given address.
 *  Notes:
 *      Immediate space required: 2 bytes
 *      Local space required: 16 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel8                // 2 bytes; rel8 is the offset to the address of the first original instruction after the injection point
 *      Registers preserved? No
 *              User must start their code with POP %rax and end their code with PUSH %rax (before the final JMP instruction)
 */
void injectJmp_2B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
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
        if(oldProtection == NULL){
            uint32_t oldProt; // If the user passes NULL for oldProtection, use &oldProt (otherwise VirtualProtect fails)
            return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)&oldProt);
        } // Else...
        return !VirtualProtect(address, size, (DWORD)newProtection, (DWORD*)oldProtection);
    
    #else // _WIN32 not defined

        // Unix (use mprotect)
        oldProtection = NULL; // This line is to avoid compiler errors; oldProtection is not used on Unix systems @TODO: implement oldProtection for Unix
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


// Writes bytecode for the series of instructions to perform an abolute JMP r64 (using JMP %rax)
//  and restore the register upon returning. Also overwrites remaining garbage bytecode with
//  the specified number of NOP instructions.
int writeJmpRax_14B(void *writeTo, void *jmpTo, int nopCount)
{
    int writeOffset = 0; // Keep track of the offset to write to (relative to the injection point)

    // Begin writing at the injection point...
    //
    // PUSH %rax:
    *(uint8_t*)writeTo = PUSH_RAX_INSTR;
    writeOffset += PUSH_RAX_INSTR_LENGTH;
    //
    //
    // MOVABS %rax, imm64:
    *(uint16_t*)((uint8_t*)writeTo + writeOffset) = *(uint16_t*)MOVABS_RAX_INSTR_OPCODE; // Opcode of MOVABS %rax, imm64
    writeOffset += MOVABS_OPCODE_LENGTH;
    #ifdef _MSC_VER
        // Using a Microsoft compiler; jump straight to the injected function:
        *(uint64_t*)((uint8_t*)writeTo + writeOffset) = (uint64_t)jmpTo; // Operand of MOVABS %rax, imm64
    #else
        // Using non-MS compiler; GCC in-line ASM starts +4 bytes from asmCode:
        *(uint64_t*)((uint8_t*)writeTo + writeOffset) = (uint64_t)((uint8_t*)jmpTo+4); // Operand of MOVABS %rax, imm64
    #endif // _MSC_VER
    writeOffset += MOVABS_OPERAND_LENGTH;
    //
    //
    // JMP %rax:
    *(uint16_t*)((uint8_t*)writeTo + writeOffset) = *(uint16_t*)JMP_ABS_RAX_INSTR;
    writeOffset += JMP_ABS_RAX_INSTR_LENGTH;
    //
    //
    // POP %rax:
    *(uint8_t*)((uint8_t*)writeTo + writeOffset) = POP_RAX_INSTR;

    // Erase trailing garbage bytes from overwritten instruction(s):
    memset((void*)((uint8_t*)writeTo + writeOffset + POP_RAX_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);

    return writeOffset;
}



// Helper function that writes bytecode for 2-byte JMP injections and the
//  local trampoline functions they utilize.
void writeBytecode_2B(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo)
{
    // Write the injected JMP rel8 instruction:
    writeJmpRel8(injectionAddr, localTrampoline, nopCount);

    // Create the local trampoline function:
    int retJmpOffset = writeJmpRax_14B(localTrampoline, (void*)jmpTo, trampNopCount+JMP_REL8_INSTR_LENGTH); // Extra NOPs because some NOPs will be overwritten with the "JMP rel8" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel8 instruction (relative to localTrampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    writeJmpRel8((uint8_t*)localTrampoline+retJmpOffset, (uint8_t*)injectionAddr+JMP_REL8_INSTR_LENGTH, trampNopCount);

}



// Writes a JMP rel8 instruction from writeTo to jmpTo, and inserts trailing NOPs if necessary
void writeJmpRel8(void *writeTo, void *jmpTo, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)writeTo+1) = (int8_t)calculateJmpOffset(writeTo, jmpTo, JMP_REL8_INSTR_LENGTH); // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}


// Writes a JMP rel8 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void writeJmpRel8(void *writeTo, int8_t offset, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)writeTo+1) = offset; // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a JMP rel32 instruction from writeTo to jmpTo, and inserts trailing NOPs if necessary
void writeJmpRel32(void *writeTo, void *jmpTo, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)writeTo+1) = (int32_t)calculateJmpOffset(writeTo, jmpTo, JMP_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a JMP rel32 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void writeJmpRel32(void *writeTo, int32_t offset, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)writeTo+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}

