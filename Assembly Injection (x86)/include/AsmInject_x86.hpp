// Made by Sean Pesce


#pragma once


#ifndef ASM_INJECT_X86_HPP
    #define ASM_INJECT_X86_HPP

#include <cstdint>  // uint8_t, uint32_t, uint16_t, int8_t, int32_t
#include <cstring>  // size_t, memset


//////////// Constants ////////////
const	uint8_t	NOP_INSTR_OPCODE = 0x90,   		// NOP instruction byte value
				JMP_REL8_INSTR_OPCODE = 0xEB,	// JMP rel8 opcode byte value
				JMP_REL32_INSTR_OPCODE = 0xE9,	// JMP rel32 opcode byte value
				CALL_REL32_INSTR_OPCODE = 0xE8, // CALL rel32 opcode byte value
				RET_INSTR_OPCODE = 0xC3,		// Near return (RET) instruction byte value
				RET_FAR_INSTR_OPCODE = 0xCB,	// Far return (RET) instruction byte value
				RET_IMM16_INSTR_OPCODE = 0xC2,		// Near return, pop imm16 bytes from stack (RET imm16) opcode value
				RET_FAR_IMM16_INSTR_OPCODE = 0xCA;	// Far return, pop imm16 bytes from stack (RET imm16) opcode value
//
const	int		JMP_REL8_INSTR_LENGTH  = 2,		// The length of a 'JMP rel8' instruction (in bytes)
				JMP_REL32_INSTR_LENGTH = 5,		// The length of a 'JMP rel32' instruction (in bytes)
				CALL_REL32_INSTR_LENGTH = 5,	// CALL rel32 instruction
				RET_INSTR_LENGTH = 1,			// RET instruction
				RET_IMM16_INSTR_LENGTH = 3;		// RET imm16 instruction


//////////// Function prototypes and descriptions ////////////


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


/* writeJmpRel8
 *  Writes a JMP rel8 instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param jmpTo    The address where the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %eip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void writeJmpRel8(void *writeTo, void *jmpTo, int nopCount);


/* writeJmpRel8
 *  Writes a JMP rel8 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel8 instruction will be written.
 *  @param offset   The offset that the JMP rel8 instruction will jump to. After the JMP rel8
 *                  instruction executes, %eip = writeTo+offset+2.
 *                  NOTE: offset must be in the range [writeTo-128,writeTo+127]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel8 instruction.
 */
void writeJmpRel8(void *writeTo, int8_t offset, int nopCount);


/* writeJmpRel32
 *  Writes a JMP rel32 instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param jmpTo    The address where the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %eip = jmpTo.
 *                  NOTE: jmpTo must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void writeJmpRel32(void *writeTo, void *jmpTo, int nopCount);


/* writeJmpRel32
 *  Writes a JMP rel32 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the JMP rel32 instruction will be written.
 *  @param offset   The offset that the JMP rel32 instruction will jump to. After the JMP rel32
 *                  instruction executes, %eip = writeTo+offset+5.
 *                  NOTE: offset must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the JMP rel32 instruction.
 */
void writeJmpRel32(void *writeTo, int32_t offset, int nopCount);


/* writeCallRel32
 *  Writes a CALL rel32 instruction at the specified writeTo address.
 *
 *  @param writeTo      The address where the CALL rel32 instruction will be written.
 *  @param procedure    The address of the called procedure. After the CALL rel32
 *                      instruction executes, %eip = procedure.
 *                      NOTE: procedure must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount     The number of NOP instructions to be written after the CALL rel32 instruction.
 */
void writeCallRel32(void *writeTo, void *procedure, int nopCount);


/* writeCallRel32
 *  Writes a CALL rel32 instruction at the specified writeTo address using the given offset.
 *
 *  @param writeTo  The address where the CALL rel32 instruction will be written.
 *  @param offset   The offset of the called procedure. After the CALL rel32
 *                  instruction executes, %eip = writeTo+offset+5.
 *                  NOTE: offset must be in the range [writeTo-2³¹,writeTo+2³¹-1]
 *  @param nopCount The number of NOP instructions to be written after the CALL rel32 instruction.
 */
void writeCallRel32(void *writeTo, int32_t offset, int nopCount);


/* writeRet
 *  Writes a "near return" (RET) instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the RET instruction will be written.
 *  @param nopCount The number of NOP instructions to be written after the RET instruction.
 */
void writeRet(void *writeTo, int nopCount);


/* writeRetFar
 *  Writes a "far return" (RET) instruction at the specified writeTo address.
 *
 *  @param writeTo  The address where the RET instruction will be written.
 *  @param nopCount The number of NOP instructions to be written after the RET instruction.
 */
void writeRetFar(void *writeTo, int nopCount);


/* writeRetImm16
 *  Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction at the
 *	 specified writeTo address.
 *
 *  @param writeTo  The address where the RET imm16 instruction will be written.
 *  @param popBytes The number of bytes to pop from the stack before returning to the calling
 *					procedure.
 *  @param nopCount The number of NOP instructions to be written after the RET imm16 instruction.
 */
void writeRetImm16(void *writeTo, uint16_t popBytes, int nopCount);


/* writeRetFarImm16
 *  Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction at the
 *	 specified writeTo address.
 *
 *  @param writeTo  The address where the RET imm16 instruction will be written.
 *  @param popBytes The number of bytes to pop from the stack before returning to the calling
 *					procedure.
 *  @param nopCount The number of NOP instructions to be written after the RET imm16 instruction.
 */
void writeRetFarImm16(void *writeTo, uint16_t popBytes, int nopCount);


/* calculateJmpOffset
 *  Calculates the offset between a 'JMP rel' instruction and a target address.
 *
 *  @param fromAddress  	The address of the JMP instruction.
 *  @param toAddress    	The address of the assembly code cave.
 *	@param jmpInstrLength   The length (in bytes) of the variation of JMP instruction being used
 */
uint32_t calculateJmpOffset(void *fromAddress, void *toAddress, int jmpInstrLength);


#endif // ASM_INJECT_X86_HPP
