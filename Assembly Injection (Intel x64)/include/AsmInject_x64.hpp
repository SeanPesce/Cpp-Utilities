// Made by Sean Pesce

#pragma once

#ifndef ASM_INJECT_X64_HPP
    #define ASM_INJECT_X64_HPP

#include <cstdint>  // uint8_t, uint16_t, uint32_t, uint64_t, int64_t
#include <cstring>  // size_t, memset

#ifdef _WIN32
    #include <Windows.h>    // VirtualProtect, GetSystemInfo
#else
    // Using Unix or other non-Windows OS
    #include <sys/mman.h>   // mprotect
    #include <unistd.h>     // sysconf(_SC_PAGESIZE)
#endif // _WIN32


//////////// Constants ////////////
//
// Byte values of various opcodes and/or full instructions:
const uint8_t   JMP_SHORT_INSTR_OPCODE = 0xEB,  // Short JMP opcode byte value (JMP rel8)
                JMP_NEAR_INSTR_OPCODE = 0xE9,   // Near JMP opcode byte value (JMP rel32)
                JMP_ABS_RAX_INSTR[2] = { 0xFF,  // Absolute JMP byte value (JMP r/m64). NOTE: Can vary, but we will only use JMP %rax
                                        0xE0 },
                NOP_INSTR_OPCODE = 0x90,        // NOP opcode byte value
                PUSH_RAX_INSTR = 0x50,          // PUSH %rax instruction value
                MOVABS_RAX_INSTR_OPCODE[2] = { 0x48,   // MOVABS %rax, imm64 opcode value
                                                0xB8 };
//
// Length (in bytes) of various instruction opcodes:
const int   JMP_SHORT_INSTR_LENGTH = 2,         // 'JMP short' (JMP rel8) instruction
            JMP_NEAR_INSTR_LENGTH = 5,          // 'JMP near' (JMP rel32) instruction
            JMP_ABS_RAX_INSTR_LENGTH = 2,       // JMP r/m64 instruction. NOTE: JMP r/m64 instuction lengths can vary, but we will only use JMP %rax
            PUSH_RAX_INSTR_LENGTH = 1,          // PUSH %rax instruction. NOTE: PUSH r64 instuction lengths can vary, but we will only use PUSH %rax
            MOVABS_OPCODE_LENGTH = 2,           // MOVABS r64, imm64 instruction opcode
            MOVABS_OPERAND_LENGTH = 8,          // MOVABS r64, imm64 instruction operand
            MOVABS_INSTR_LENGTH = 10;           // MOVABS r64, imm64 instruction
//
//
// Important values from the intermediate trampoline function that is called when the user calls injectJmp_13B():
#ifdef _MSC_VER
    // Visual Studio doesn't support in-line ASM on 64-bit projects; use an external .asm file
    extern "C" uint64_t * const TRAMPOLINE_JMP_TO_PTR;
    extern "C" uint64_t * const TRAMPOLINE_RET_TO_PTR;
    const int TRAMPOLINE_JMPBACK_INSTR_OFFSET = 12; // Offset of the trampoline function's returning JMP instruction from the beginning of the function
#else
    extern uint64_t *TRAMPOLINE_JMP_TO_PTR;
    extern uint64_t *TRAMPOLINE_RET_TO_PTR;
    const int TRAMPOLINE_JMPBACK_INSTR_OFFSET = 12;
#endif // _MSC_VER

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

/* TRAMPOLINE_FUNC_13B
 *  ASM trampoline function that is called as a wrapping function for a user's 
 *      ASM code when that user calls injectJmp_13B(). The trampoline function is called
 *      first because we push %rax to the stack at the injection point to free a register 
 *      for the JMP r/m64 call, and we don't want the user to worry about popping %rax to
 *      preserve it and/or alter its contents. To inject without a trampoline function, use
 *      injectJmp_13B_Unsafe().
 */
#ifdef _MSC_VER
    // Visual Studio doesn't support in-line ASM on 64-bit projects; use an external .asm file
    extern "C" void * const TRAMPOLINE_FUNC_13B;
#else
    void TRAMPOLINE_FUNC_13B();
#endif // _MSC_VER



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



/* calculateJmpOffset
 *  Calculates the offset between a JMP rel instruction and toAddress.
 *  @param fromAddress      The address of the JMP instruction.
 *  @param toAddress        The address to jump to.
 *  @param jmpInstrLength   The length of the variation of JMP instruction being used
 */
int64_t calculateJmpOffset(void *fromAddress, void *toAddress, int jmpInstrLength);



/* getMemPage
 *  Obtains the starting address of the page of memory that contains a given memory address.
 *  @param memAddress   The given memory address that resides within the page.
 */
void *getMemPage(void *memAddress);



/* injectJmp_13B
 *  Injects an absolute JMP instruction (JMP r/m64) at the given address.
 *
 * Notes:
 *  Space required: 13 bytes
 *  Trampoline function used?   Yes
 *  Registers preserved?        Yes
 *
 * Injected code:
 *      PUSH %rax
 *      MOVABS %rax, imm64      // imm64 is the address of the trampoline function
 *      JMP %rax
 *
 *  @param injectionAddr    The location in memory where the assembly code will be injected. A
 *                          JMP instruction will be written at this location.
 *  @param returnJmpAddr    The location in memory where the code cave should return to
 *                          after execution. This function writes to returnJmpAddr.
 *  @param nopCount         The number of NOP instructions to be written after the injected code.
 *                          These NOPs will erase any remaining garbage bytes resulting from
 *                          overwriting existing instructions at the injection location.
 *  @param asmCode  A pointer to an assembly function (to be used as a code cave).
 */
void injectJmp_13B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode);



/* injectJmp_13B_Unsafe
 *  Injects an absolute JMP instruction (JMP r/m64) at the given address. This method is faster
 *      than the safe injectJmp_13B function because it doesn't jump to a trampoline function
 *      to automatically preserve %rax. Users should start an injected function with "POP %rax"
 *      when using this injection method.
 *  
 *  WARNING: Injection functions marked as "_Unsafe" don't preserve registers and/or require the user
 *        to start their injected function with specific instructions to avoid data corruption.
 *
 * Notes:
 *  Space required: 13 bytes
 *  Trampoline function used?   No
 *  Registers preserved?        No, %rax must be popped at beginning of user's ASM code
 *  Necessary inclusion(s) to user code:
 *      POP %rax
 *  
 * Injected code:
 *      PUSH %rax
 *      MOVABS %rax, imm64      //imm64 is the address of the injected function
 *      JMP %rax
 *
 * @params: See documentation for @injectJmp_13B
 */
void injectJmp_13B_Unsafe(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode);



 /* writeBytecode_13B
  *
  *  Helper function that writes the bytecode for 13-byte JMP injections and overwrites remaining
  *     garbage bytecode with the specified number of NOP instructions.
  *  Bytecode:
  *     PUSH %rax
  *     MOVABS %rax, imm64      // imm64 is the address of the injected code or a trampoline function
  *     JMP %rax
  */
int writeBytecode_13B(void *injectionAddr, int nopCount, void *jmpTo);



/* injectJmp_2B
 *  Injects an relative JMP instruction (JMP rel8) at the given address. The JMP rel8 instruction
 *      jumps to a local code cave with a single instruction, which is another jump instruction.
 *      The second jump instruction is a JMP r/m64, and jumps to an absolute 64-bit address (8 bytes)
 *      held by local pointer. This is the smallest possible code injection, at only 2 bytes, but
 *      it requires at least 13 bytes of local storage.
 *
 * Notes:
 *  Injection space required: 2 bytes
 *  Local space required: 14 bytes (total)
 *      6 bytes (JMP r/m64)
 *      8 bytes (absolute 64-bit address)
 *  Trampoline function used?   No
 *  Registers preserved?        Yes
 *
 * Injected code:
 *      JMP rel8        // rel8 is the relative offset of the local JMP r/m64 instruction
 *
 *  @param injectionAddr    The location in memory where the assembly code will be injected. A
 *                          JMP instruction will be written at this location.
 *  @param returnJmpAddr    The location in memory where the code cave should return to
 *                          after execution. This function writes to returnJmpAddr.
 *  @param nopCount         The number of NOP instructions to be written after the injected code.
 *                          These NOPs will erase any remaining garbage bytes resulting from
 *                          overwriting existing instructions at the injection location.
 *  @param asmCode          A pointer to an assembly function (to be used as a code cave).
 *  @param chainJmp         The address of the local JMP r/m64 instruction, which must be in the range:
 *                          [injectionAddr+129, injectionAddr-126]
 *  @param chainJmpNopCount The number of NOP instructions to be written after the chained JMP. This might
 *                          be necessary if the user wrote their chainJmp over existing instructions.
 *  @param chainPtr         The address of the local pointer which will hold the absolute address of the
 *                          user's code cave (same as asmCode). chainPtr must be in the range:
 *                          [chainJmp+2³¹+5, chainJmp-2³¹+6]
 *  @param chainPtrNopCount The number of NOP instructions to be written after the absolute address specified by chainPtr.
 *                          This might be necessary if the user wrote their absolute address over existing instructions.
 */
void injectJmp_2B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                    void *chainJmp, int chainJmpNopCount, void *chainPtr, int chainPtrNopCount); // @TODO

 

#endif // ASM_INJECT_X64_HPP