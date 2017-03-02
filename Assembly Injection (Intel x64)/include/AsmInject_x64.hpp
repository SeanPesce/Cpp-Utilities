// Made by Sean Pesce

#pragma once

#ifndef ASM_INJECT_X64_HPP
    #define ASM_INJECT_X64_HPP

#include <cstdint>  // uint8_t, int8_t, uint16_t, uint32_t, uint64_t, int64_t
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
const uint8_t   JMP_REL8_INSTR_OPCODE = 0xEB,   // Short JMP opcode byte value (JMP rel8)
                JMP_REL32_INSTR_OPCODE = 0xE9,  // Near JMP opcode byte value (JMP rel32)
                JMP_ABS_RAX_INSTR[2] = { 0xFF,  // Absolute JMP byte value (JMP r/m64). NOTE: Can vary, but we will only use JMP %rax
                                        0xE0 },
                NOP_INSTR_OPCODE = 0x90,        // NOP opcode byte value
                PUSH_RAX_INSTR = 0x50,          // PUSH %rax instruction byte value
                POP_RAX_INSTR = 0x58,           // POP %rax instruction byte value
                MOVABS_RAX_INSTR_OPCODE[2] = { 0x48,   // MOVABS %rax, imm64 opcode value
                                                0xB8 };
//
// Length (in bytes) of various instruction opcodes:
const int   JMP_REL8_INSTR_LENGTH = 2,          // 'JMP short' (JMP rel8) instruction
            JMP_REL32_INSTR_LENGTH = 5,         // 'JMP near' (JMP rel32) instruction
            JMP_ABS_RAX_INSTR_LENGTH = 2,       // JMP r/m64 instruction. NOTE: JMP r/m64 instuction lengths can vary, but we will only use JMP %rax
            PUSH_RAX_INSTR_LENGTH = 1,          // PUSH %rax instruction. NOTE: PUSH r64 instuction lengths can vary, but we will only use PUSH %rax
            POP_RAX_INSTR_LENGTH = 1,           // POP %rax instruction. NOTE: POP r64 instuction lengths can vary, but we will only use POP %rax
            MOVABS_OPCODE_LENGTH = 2,           // MOVABS r64, imm64 instruction opcode
            MOVABS_OPERAND_LENGTH = 8,          // MOVABS r64, imm64 instruction operand
            MOVABS_INSTR_LENGTH = 10;           // MOVABS r64, imm64 instruction


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



/* injectJmp_14B
 *  Injects code using an absolute JMP instruction (JMP r/m64) at the given address. Users
 *      should start an injected function with "POP %rax" and use "PUSH %rax" before their
 *      final return JMP when using this injection method.
 *  
 *  WARNING: This injection functions is unsafe; it doesn't preserve %rax and requires the user
 *        to format their injected function with specific instructions to avoid data corruption.
 *
 * Notes:
 *  Space required: 14 bytes
 *  Registers preserved?        No
 *                              User should start code with POP %rax
 *                              User should call PUSH %rax before final returning JMP instruction
 *  
 *  Necessary inclusion(s) to user code:
 *      POP %rax    // Beginning of user code
 *      // User code body
 *      PUSH %rax   // End of user code
 *      MOVABS %rax, returnJmpAddr
 *      JMP %rax
 *  
 * Injected code:
 *      PUSH %rax
 *      MOVABS %rax, imm64      // imm64 is the address of the injected function
 *      JMP %rax
 *      POP %rax                // User code should return here
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
void injectJmp_14B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode);



 /* writeJmpRax_14B
  *
  *  Writes bytecode for the series of instructions to perform an abolute JMP r64 (using JMP %rax)
  *     and restore the register upon returning. Also overwrites remaining garbage bytecode with
  *     the specified number of NOP instructions.
  *  Bytecode:
  *     PUSH %rax
  *     MOVABS %rax, imm64      // imm64 is the address to jump to
  *     JMP %rax
  *     POP %rax                // %rax is restored upon returning here
  */
int writeJmpRax_14B(void *injectionAddr, void *jmpTo, int nopCount);



/* injectJmp_2B
 *  Injects an relative JMP instruction (JMP rel8) at the given address. The JMP rel8 instruction
 *      jumps to a local code cave with an injectJmp_14B instruction sequence, seen below.
 *      The second jump instruction is a JMP r/m64, and jumps to an absolute 64-bit address (8 bytes)
 *      inserted into %rax. This is the smallest possible code injection, at only 2 bytes, but
 *      it requires at least 16 bytes of local storage.
 *
 *  WARNING: This injection functions is unsafe; it doesn't preserve %rax and requires the user
 *        to format their injected function with specific instructions to avoid data corruption.
 *
 * Notes:
 *  Injection space required: 2 bytes
 *  Local space required: 16 bytes (local code cave)
 *      PUSH %rax               // 1 byte
 *      MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *      JMP %rax                // 2 bytes
 *      POP %rax                // 1 byte
 *      JMP rel8                // 2 bytes; rel8 is the offset to the address of the first original instruction after the injection point
 *  Registers preserved?        No
 *                              User should start code with POP %rax
 *                              User should call PUSH %rax before final returning JMP instruction
 *  
 *  Necessary inclusion(s) to user code:
 *      POP %rax    // Beginning of user code
 *      // User code body
 *      PUSH %rax   // End of user code
 *      MOVABS %rax, returnJmpAddr
 *      JMP %rax
 *
 * Injected code:
 *      JMP rel8        // rel8 is the relative offset of the local trampoline with a JMP r/m64 instruction
 *
 *  @param injectionAddr    The location in memory where the assembly code will be injected. A
 *                          JMP instruction will be written at this location.
 *  @param returnJmpAddr    The location in memory where the code cave should return to
 *                          after execution. This function writes to returnJmpAddr.
 *  @param nopCount         The number of NOP instructions to be written after the injected code.
 *                          These NOPs will erase any remaining garbage bytes resulting from
 *                          overwriting existing instructions at the injection location.
 *  @param asmCode          A pointer to an assembly function (to be used as a code cave).
 *  @param localTrampoline  The address of the local trampoline function (structured the same as a
 *                          injectJmp_14B), which must start in the range:
 *                          [injectionAddr+129, injectionAddr-126]
 *  @param trampNopCount    The number of NOP instructions to be written after the local trampoline.
 *                          This could be necessary if the user wrote their trampoline function over
 *                          existing instructions
 */
void injectJmp_2B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                    void *localTrampoline, int trampNopCount);



/* injectJmp_5B
 *  Injects an relative JMP instruction (JMP rel32) at the given address. The JMP rel32 instruction
 *      jumps to a local code cave with an injectJmp_14B instruction sequence, seen below.
 *      The second jump instruction is a JMP r/m64, and jumps to an absolute 64-bit address (8 bytes)
 *      inserted into %rax. This code injection technique requires only 5 immediate bytes, but
 *      it requires at least 19 bytes of local storage.
 *
 *  WARNING: This injection functions is unsafe; it doesn't preserve %rax and requires the user
 *        to format their injected function with specific instructions to avoid data corruption.
 *
 * Notes:
 *  Injection space required: 5 bytes
 *  Local space required: 19 bytes (local code cave)
 *      PUSH %rax               // 1 byte
 *      MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *      JMP %rax                // 2 bytes
 *      POP %rax                // 1 byte
 *      JMP rel32                // 5 bytes; rel32 is the offset to the address of the first original instruction after the injection point
 *  Registers preserved?        No
 *                              User should start code with POP %rax
 *                              User should call PUSH %rax before final returning JMP instruction
 *  
 *  Necessary inclusion(s) to user code:
 *      POP %rax    // Beginning of user code
 *      // User code body
 *      PUSH %rax   // End of user code
 *      MOVABS %rax, returnJmpAddr
 *      JMP %rax
 *
 * Injected code:
 *      JMP rel32        // rel32 is the relative offset of the local trampoline with a JMP r/m64 instruction
 *
 *  @param injectionAddr    The location in memory where the assembly code will be injected. A
 *                          JMP instruction will be written at this location.
 *  @param returnJmpAddr    The location in memory where the code cave should return to
 *                          after execution. This function writes to returnJmpAddr.
 *  @param nopCount         The number of NOP instructions to be written after the injected code.
 *                          These NOPs will erase any remaining garbage bytes resulting from
 *                          overwriting existing instructions at the injection location.
 *  @param asmCode          A pointer to an assembly function (to be used as a code cave).
 *  @param localTrampoline  The address of the local trampoline function (structured the same as a
 *                          injectJmp_14B), which must start in the range:
 *                          [injectionAddr-2³¹+5,injectionAddr+2³¹+4]
 *  @param trampNopCount    The number of NOP instructions to be written after the local trampoline.
 *                          This could be necessary if the user wrote their trampoline function over
 *                          existing instructions
 */
void injectJmp_5B(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                    void *localTrampoline, int trampNopCount);



/* Helper function that does the following for 2-byte JMP injections:
 *      -Writes the bytecode for the JMP rel8 instruction.
 *      -Overwrites remaining garbage bytecode with the specified number of NOP instructions.
 *      -Creates the local trampoline function at the specified location.
 *      -Overwrites remaining garbage bytecode with NOPs if local trampoline was written over
 *          existing instructions.
 */
void writeBytecode_2B(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo);



/* Helper function that does the following for 5-byte JMP injections:
 *      -Writes the bytecode for the JMP rel32 instruction.
 *      -Overwrites remaining garbage bytecode with the specified number of NOP instructions.
 *      -Creates the local trampoline function at the specified location.
 *      -Overwrites remaining garbage bytecode with NOPs if local trampoline was written over
 *          existing instructions.
 */
void writeBytecode_5B(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo);



/* writeJmpRel8
 *  Writes a JMP rel8 instruction from the specified writeTo address to the given jmpTo address.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param jmpTo    The address where the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %rip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void writeJmpRel8(void *writeTo, void *jmpTo, int nopCount);



/* writeJmpRel8
 *  Writes a JMP rel8 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param offset   The offset that the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %rip = writeTo+offset+2.
 *                  NOTE: offset must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void writeJmpRel8(void *writeTo, int8_t offset, int nopCount);



/* writeJmpRel32
 *  Writes a JMP rel32 instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param jmpTo    The address where the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %rip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void writeJmpRel32(void *writeTo, void *jmpTo, int nopCount);



/* writeJmpRel32
 *  Writes a JMP rel32 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param offset   The offset that the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %rip = writeTo+offset+5.
 *                  NOTE: offset must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void writeJmpRel32(void *writeTo, int32_t offset, int nopCount);
 

#endif // ASM_INJECT_X64_HPP