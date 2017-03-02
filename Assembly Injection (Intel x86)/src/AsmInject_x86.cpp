// Made by Sean Pesce

/* Credits:
 *  Some portions of this code were inspired by or altered from code
 *  written by atom0s from the CheatEngine forums. User page for atom0s
 *  can be found here: 
 *  http://www.cheatengine.org/forum/profile.php?mode=viewprofile&u=8422&sid=9e8a3b96f0c5924b7a2fc85204bb8704
 */

#include "AsmInject_x86.hpp"



void injectASM(uint8_t *injectionAddr, uint32_t *returnJumpAddr, int nopCount, void *asmCode)
{

    // The remaining 4 bytes of the instruction are the operand, specifying the offset from this address to the code cave:
    #ifdef _MSC_VER
        // Using a Microsoft compiler; jump straight to the code cave:
		writeJmpRel32(injectionAddr, asmCode, nopCount);
    #else
        // Using non-MS compiler; GCC ASM starts +3 bytes from &asmCode:
        writeJmpRel32(injectionAddr, (uint8_t*)asmCode+3, nopCount);
    #endif // _MSC_VER

    // Calculate the address of the next instruction after the injected JMP and write it to the in-line ASM function's return JMP:
    *returnJumpAddr = ((uint32_t)injectionAddr + JMP_REL32_INSTR_LENGTH);
}



// Writes a JMP rel8 instruction from writeTo to jmpTo, and inserts trailing NOPs (if necessary):
void writeJmpRel8(void *writeTo, void *jmpTo, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((uint8_t*)writeTo+1) = (int8_t)calculateJmpOffset(writeTo, jmpTo, JMP_REL8_INSTR_LENGTH); // Write operand byte

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



// Writes a JMP rel32 instruction from writeTo to jmpTo, and inserts trailing NOPs (if necessary):
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



// Calculates the offset between a JMP instruction and target address:
uint32_t calculateJmpOffset(void *fromAddress, void *toAddress, int jmpInstrLength)
{
    return ((uint32_t)toAddress - (uint32_t)fromAddress - jmpInstrLength);
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
        uint32_t pageSize = (uint32_t)sysInfo.dwPageSize;
    #else // Unix:
        uint32_t pageSize = (uint32_t)sysconf(_SC_PAGESIZE);
    #endif // _WIN32
    
    // Calculate the base address of the page and return it:
    return (void*)((uint32_t)memAddress - ((uint32_t)memAddress % pageSize));
}

