// Made by Sean Pesce


#pragma once


#ifndef ASM_INJECT_X86_HPP
    #define ASM_INJECT_X86_HPP

#include <cstdint>  // uint8_t, uint32_t
#include <cstring>  // size_t, memset

#ifdef _WIN32
    #include <Windows.h>    // VirtualProtect, GetSystemInfo
#else
    // Using Unix or other non-Windows OS
    #include <sys/mman.h>   // mprotect
    #include <unistd.h>     // sysconf(_SC_PAGESIZE)
#endif // _WIN32


//////////// Constants ////////////
const uint8_t JMP_INSTR_OPCODE = 0xE9;   // JMP opcode byte value (JMP rel32)
const uint8_t NOP_INSTR_OPCODE = 0x90;   // NOP opcode byte value
const int JMP_INSTR_LENGTH = 5;   // The length of a 'JMP rel32' instruction (in bytes).

// Portable memory protection setting constants:
#ifdef _WIN32
    // NOTE: Not all Windows protection settings are included. See Microsoft documentation for more info:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366786(v=vs.85).aspx
    #define MEM_PROTECT_NONE PAGE_NOACCESS
    #define MEM_PROTECT_R PAGE_READONLY
    #define MEM_PROTECT_W PAGE_WRITECOPY
    #define MEM_PROTECT_X PAGE_EXECUTE
    #define MEM_PROTECT_RW PAGE_READWRITE
    #define MEM_PROTECT_RX PAGE_EXECUTE_READ
    #define MEM_PROTECT_WX PAGE_EXECUTE_WRITECOPY
    #define MEM_PROTECT_RWX PAGE_EXECUTE_READWRITE
#else
    #define MEM_PROTECT_NONE PROT_NONE
    #define MEM_PROTECT_R PROT_READ
    #define MEM_PROTECT_W PROT_WRITE
    #define MEM_PROTECT_X PROT_EXEC
    #define MEM_PROTECT_RW PROT_READ|PROT_WRITE
    #define MEM_PROTECT_RX PROT_READ|PROT_EXEC
    #define MEM_PROTECT_WX PROT_WRITE|PROT_EXEC
    #define MEM_PROTECT_RWX PROT_READ|PROT_WRITE|PROT_EXEC
#endif // _WIN32


//////////// Function prototypes and descriptions ////////////

/* SET_MEM_PROTECTION
 *  Cross-platform wrapper function for changing memory protection settings.
 *      On Windows, VirtualProtect is called. On Unix, mprotect is called.
 *  @param address  The location in memory where the memory protection will be changed.
 *  @param size The size of the section of memory whose protection will be changed.
 *              In Windows, the size is measured in bytes.
 *              In Unix, the size is measured in pages.
 *  @param newProtection    The new memory protection setting for the section of memory.
 *  @param oldProtection    Saves the original memory protection settings on Windows.
                            NOTE: The oldProtection parameter is not used in Unix.
 *  @return 0 if successful; non-zero value on failure.
 */
int SET_MEM_PROTECTION(void *address, size_t size, uint32_t newProtection, uint32_t *oldProtection);

/* injectASM
 *  Injects assembly code at the specified location in memory.
 *		NOTE:	Memory at the injection point must be writable before calling this function (use
 *				SET_MEM_PROTECTION).
 *		NOTE:	There must be AT LEAST 5 bytes of memory at the injection point that is either
 *				unused or that can be overwritten without disrupting program logic; this space
 *				is necessary to write the JMP instruction that directs execution to the injected
 *				assembly code.
 *  @param injectionAddr    The location in memory where the assembly code will be injected. A
 *                          JMP instruction will be written at this location, containing the
 *                          address of the assembly function referenced by the asmCode parameter.
 *  @param returnJumpAddr   The location in memory where the code cave should return to
 *                          after execution. returnJumpAddr should point to a uin32_t that
 *                          is referenced in a JMP instruction at the end of the assembly
 *                          function referenced by the asmCode parameter.
 *  @param nopCount         The number of NOP instructions to be written after the initial JMP
 *                          instruction. These NOPs will erase any remaining garbage bytes
 *                          that resulted from overwriting existing instructions at the injection
 *                          location.
 *  @param asmCode  A pointer to an in-line assembly function (to be used as a code cave).
 */
void injectASM(uint8_t *injectionAddr, uint32_t *returnJumpAddr, int nopCount, void *asmCode);

/* calculateJmpOffset
 *  Calculates the offset between an injected 'JMP rel32' instruction and an ASM code cave.
 *      'JMP rel32' instructions are defined as follows:
 *          OPCODE:   MNEMONIC:   DESCRIPTION:
 *          E9 cd     JMP rel32   Jump near, relative, displacement relative to next instruction.
 *  @param fromAddress  The address of the JMP instruction.
 *  @param toAddress    The address of the assembly code cave.
 */
uint32_t calculateJmpOffset(void *fromAddress, void *toAddress);

/* getMemPage
 *  Obtains the starting address of the page of memory that contains a given memory address.
 *  @param memAddress   The given memory address that resides within the page.
 */
void *getMemPage(void *memAddress);

#endif // ASM_INJECT_X86_HPP
