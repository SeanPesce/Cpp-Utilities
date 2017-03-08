// Made by Sean Pesce

#pragma once

#ifndef ASM_INJECT_X64_HPP
    #define ASM_INJECT_X64_HPP

#include <cstdint>  // uint8_t, int8_t, uint16_t, uint32_t, int32_t, uint64_t, int64_t
#include <cstring>  // size_t, memset


//////////// Constants ////////////
//
// Byte values of various opcodes and/or full instructions:
const uint8_t   JMP_REL8_INSTR_OPCODE = 0xEB,   // Short JMP opcode byte value (JMP rel8)
                JMP_REL32_INSTR_OPCODE = 0xE9,  // Near JMP opcode byte value (JMP rel32)
                JMP_ABS_RAX_INSTR[2] = { 0xFF,  // Absolute JMP byte value (JMP r/m64). NOTE: Can vary, but we will only use JMP %rax
                                        0xE0 },
                CALL_REL32_INSTR_OPCODE = 0xE8, // CALL rel32 opcode byte value
                RET_INSTR_OPCODE = 0xC3,		// Near return (RET) instruction byte value
				RET_FAR_INSTR_OPCODE = 0xCB,	// Far return (RET) instruction byte value
				RET_IMM16_INSTR_OPCODE = 0xC2,		// Near return, pop imm16 bytes from stack (RET imm16) opcode value
				RET_FAR_IMM16_INSTR_OPCODE = 0xCA,	// Far return, pop imm16 bytes from stack (RET imm16) opcode value
                NOP_INSTR_OPCODE = 0x90,        // NOP opcode byte value
                PUSH_RAX_INSTR = 0x50,          // PUSH %rax instruction byte value
                POP_RAX_INSTR = 0x58,           // POP %rax instruction byte value
                MOVABS_RAX_INSTR_OPCODE[2] = { 0x48,   // MOVABS %rax, imm64 opcode value
                                                0xB8 };
//
// Length (in bytes) of various instruction opcodes:
const int   JMP_REL8_INSTR_LENGTH = 2,      // 'JMP short' (JMP rel8) instruction
            JMP_REL32_INSTR_LENGTH = 5,     // 'JMP near' (JMP rel32) instruction
            JMP_ABS_RAX_INSTR_LENGTH = 2,   // JMP r/m64 instruction. NOTE: JMP r/m64 instuction lengths can vary, but we will only use JMP %rax
            CALL_REL32_INSTR_LENGTH = 5,    // CALL rel32 instruction
			RET_INSTR_LENGTH = 1,           // RET instruction
			RET_IMM16_INSTR_LENGTH = 3,     // RET imm16 instruction
            PUSH_RAX_INSTR_LENGTH = 1,      // PUSH %rax instruction. NOTE: PUSH r64 instuction lengths can vary, but we will only use PUSH %rax
            POP_RAX_INSTR_LENGTH = 1,       // POP %rax instruction. NOTE: POP r64 instuction lengths can vary, but we will only use POP %rax
            MOVABS_OPCODE_LENGTH = 2,       // MOVABS r64, imm64 instruction opcode
            MOVABS_OPERAND_LENGTH = 8,      // MOVABS r64, imm64 instruction operand
            MOVABS_INSTR_LENGTH = 10;       // MOVABS r64, imm64 instruction



//////////// Function prototypes and descriptions ////////////



/* inject_jmp_14b
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
void inject_jmp_14b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode);



/* inject_jmp_5b
 *  Injects an relative JMP instruction (JMP rel32) at the given address. The JMP rel32 instruction
 *      jumps to a local code cave with an inject_jmp_14b instruction sequence, seen below.
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
 *                          inject_jmp_14b), which must start in the range:
 *                          [injectionAddr-2³¹+5,injectionAddr+2³¹+4]
 *  @param trampNopCount    The number of NOP instructions to be written after the local trampoline.
 *                          This could be necessary if the user wrote their trampoline function over
 *                          existing instructions
 */
void inject_jmp_5b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                    void *localTrampoline, int trampNopCount);



/* inject_jmp_2b
 *  Injects an relative JMP instruction (JMP rel8) at the given address. The JMP rel8 instruction
 *      jumps to a local code cave with an inject_jmp_14b instruction sequence, seen below.
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
 *                          inject_jmp_14b), which must start in the range:
 *                          [injectionAddr+129, injectionAddr-126]
 *  @param trampNopCount    The number of NOP instructions to be written after the local trampoline.
 *                          This could be necessary if the user wrote their trampoline function over
 *                          existing instructions
 */
void inject_jmp_2b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                    void *localTrampoline, int trampNopCount);



/* Helper function that does the following for 5-byte JMP injections:
 *      -Writes the bytecode for the JMP rel32 instruction.
 *      -Overwrites remaining garbage bytecode with the specified number of NOP instructions.
 *      -Creates the local trampoline function at the specified location.
 *      -Overwrites remaining garbage bytecode with NOPs if local trampoline was written over
 *          existing instructions.
 */
void write_bytecode_5b(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo);



/* Helper function that does the following for 2-byte JMP injections:
 *      -Writes the bytecode for the JMP rel8 instruction.
 *      -Overwrites remaining garbage bytecode with the specified number of NOP instructions.
 *      -Creates the local trampoline function at the specified location.
 *      -Overwrites remaining garbage bytecode with NOPs if local trampoline was written over
 *          existing instructions.
 */
void write_bytecode_2b(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo);



 /* write_jmp_rax_14b
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
int write_jmp_rax_14b(void *injectionAddr, void *jmpTo, int nopCount);



/* write_jmp_rel8
 *  Writes a JMP rel8 instruction from the specified writeTo address to the given jmpTo address.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param jmpTo    The address where the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %rip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void write_jmp_rel8(void *writeTo, void *jmpTo, int nopCount);



/* write_jmp_rel8
 *  Writes a JMP rel8 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param offset   The offset that the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %rip = writeTo+offset+2.
 *                  NOTE: offset must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void write_jmp_rel8(void *writeTo, int8_t offset, int nopCount);



/* write_jmp_rel32
 *  Writes a JMP rel32 instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param jmpTo    The address where the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %rip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void write_jmp_rel32(void *writeTo, void *jmpTo, int nopCount);



/* write_jmp_rel32
 *  Writes a JMP rel32 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param offset   The offset that the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %rip = writeTo+offset+5.
 *                  NOTE: offset must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void write_jmp_rel32(void *writeTo, int32_t offset, int nopCount);



/* write_call_rel32
 *  Writes a CALL rel32 instruction at the specified writeTo address.
 *
 *  @param writeTo      The address where the CALL rel32 instruction will be written.
 *  @param procedure    The address of the called procedure. After the CALL rel32
 *                      instruction executes, %rip = procedure.
 *                      NOTE: procedure must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount     The number of NOP instructions to be written after the CALL rel32 instruction.
 */
void write_call_rel32(void *writeTo, void *procedure, int nopCount);



/* write_call_rel32
 *  Writes a CALL rel32 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the CALL rel32 instruction will be written.
 *  @param offset   The offset of the called procedure. After the CALL rel32
 *                  instruction executes, %rip = writeTo+offset+5.
 *                  NOTE: offset must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the CALL rel32 instruction.
 */
void write_call_rel32(void *writeTo, int32_t offset, int nopCount);



/* write_ret
 *  Writes a "near return" (RET) instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the RET instruction will be written.
 *  @param nopCount The number of NOP instructions to be written after the RET instruction.
 */
void write_ret(void *writeTo, int nopCount);



/* write_ret_far
 *  Writes a "far return" (RET) instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the RET instruction will be written.
 *  @param nopCount The number of NOP instructions to be written after the RET instruction.
 */
void write_ret_far(void *writeTo, int nopCount);



/* write_ret_imm16
 *  Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction at the
 *	 specified writeTo address.
 *
 *  @param writeTo  The address where the RET imm16 instruction will be written.
 *  @param popBytes The number of bytes to pop from the stack before returning to the calling
 *					procedure.
 *  @param nopCount The number of NOP instructions to be written after the RET imm16 instruction.
 */
void write_ret_imm16(void *writeTo, uint16_t popBytes, int nopCount);



/* write_ret_far_imm16
 *  Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction at the
 *	 specified writeTo address.
 *
 *  @param writeTo  The address where the RET imm16 instruction will be written.
 *  @param popBytes The number of bytes to pop from the stack before returning to the calling
 *					procedure.
 *  @param nopCount The number of NOP instructions to be written after the RET imm16 instruction.
 */
void write_ret_far_imm16(void *writeTo, uint16_t popBytes, int nopCount);



/* calculate_jmp_offset
 *  Calculates the offset between a JMP rel instruction and toAddress.
 *  @param fromAddress      The address of the JMP instruction.
 *  @param toAddress        The address to jump to.
 *  @param jmpInstrLength   The length of the variation of JMP instruction being used
 */
int64_t calculate_jmp_offset(void *fromAddress, void *toAddress, int jmpInstrLength);

 

#endif // ASM_INJECT_X64_HPP
