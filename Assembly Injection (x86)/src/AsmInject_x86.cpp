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



// Writes a CALL rel32 instruction from writeTo to procedure, and inserts trailing NOPs if necessary
void writeCallRel32(void *writeTo, void *procedure, int nopCount)
{
    *(uint8_t*)writeTo = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)writeTo+1) = (int32_t)calculateJmpOffset(writeTo, procedure, CALL_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a CALL rel32 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void writeCallRel32(void *writeTo, int32_t offset, int nopCount)
{
    *(uint8_t*)writeTo = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)writeTo+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "near return" (RET) instruction to writeTo and inserts trailing NOPs if necessary
void writeRet(void *writeTo, int nopCount)
{
	*(uint8_t*)writeTo = RET_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "far return" (RET) instruction to writeTo and inserts trailing NOPs if necessary
void writeRetFar(void *writeTo, int nopCount)
{
	*(uint8_t*)writeTo = RET_FAR_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction to writeTo and
//  inserts trailing NOPs if necessary
void writeRetImm16(void *writeTo, uint16_t popBytes, int nopCount)
{
	*(uint8_t*)writeTo = RET_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)writeTo+1) = popBytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction to writeTo and
//  inserts trailing NOPs if necessary
void writeRetFarImm16(void *writeTo, uint16_t popBytes, int nopCount)
{
	*(uint8_t*)writeTo = RET_FAR_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)writeTo+1) = popBytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Calculates the offset between a JMP instruction and target address:
uint32_t calculateJmpOffset(void *fromAddress, void *toAddress, int jmpInstrLength)
{
    return ((uint32_t)toAddress - (uint32_t)fromAddress - jmpInstrLength);
}



