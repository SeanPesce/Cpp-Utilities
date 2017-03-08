// Made by Sean Pesce

#include "AsmInject_x64.hpp"



/* Injects code using a JMP r/m64 instruction at the given address.
 *  Notes:
 *      Space required: 14 bytes
 *      Registers preserved? No
 *          User must start their code with POP %rax and end their code with PUSH %rax/MOVABS %rax, returnJmpAddr/JMP %rax
 */
void inject_jmp_14b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode)
{
    // Write the injected bytecode and store the final write offset (relative to the injection point):
    int popRaxOffset = write_jmp_rax_14b(injectionAddr, asmCode, nopCount); // The returned offset is also the offset of the POP %rax instruction

    // Direct the user's return JMP to the POP %rax instruction:
    *(uint64_t*)returnJmpAddr = (uint64_t)((uint8_t*)injectionAddr + popRaxOffset);
}



/* Injects code using a JMP rel32 instruction at the given address.
 *  Notes:
 *      Immediate space required: 5 bytes
 *      Local space required: 19 bytes (local code cave)
 *          PUSH %rax               // 1 byte
 *          MOVABS %rax, imm64      // 10 bytes; imm64 is the address of the injected code
 *          JMP %rax                // 2 bytes
 *          POP %rax                // 1 byte
 *          JMP rel32                // 5 bytes; rel32 is the offset to the address of the first original instruction after the injection point
 *      Registers preserved? No
 *              User must start their code with POP %rax and end their code with PUSH %rax (before the final JMP instruction)
 */
void inject_jmp_5b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                  void *localTrampoline, int trampNopCount)
{
    // Write the injected JMP rel32 instruction and local trampoline:
    write_bytecode_5b(injectionAddr, nopCount, localTrampoline, trampNopCount, asmCode);

    // Obtain the offset of the POP %rax instruction in the local trampoline function:
    int popRaxOffset = PUSH_RAX_INSTR_LENGTH + MOVABS_INSTR_LENGTH + JMP_ABS_RAX_INSTR_LENGTH;

    // Direct the user's return JMP to the POP %rax instruction in the local trampoline function:
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
void inject_jmp_2b(void *injectionAddr, void *returnJmpAddr, int nopCount, void *asmCode,
                  void *localTrampoline, int trampNopCount)
{
    // Write the injected JMP rel8 instruction and local trampoline:
    write_bytecode_2b(injectionAddr, nopCount, localTrampoline, trampNopCount, asmCode);

    // Obtain the offset of the POP %rax instruction in the local trampoline function:
    int popRaxOffset = PUSH_RAX_INSTR_LENGTH + MOVABS_INSTR_LENGTH + JMP_ABS_RAX_INSTR_LENGTH;

    // Direct the user's return JMP to the POP %rax instruction in the local trampoline function:
    *(uint64_t*)returnJmpAddr = (uint64_t)((uint8_t*)injectionAddr + popRaxOffset);
}



// Helper function that writes bytecode for 5-byte JMP injections and the
//  local trampoline functions they utilize.
void write_bytecode_5b(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo)
{
    // Write the injected JMP rel32 instruction:
    write_jmp_rel32(injectionAddr, localTrampoline, nopCount);

    // Create the local trampoline function:
    int retJmpOffset = write_jmp_rax_14b(localTrampoline, (void*)jmpTo, trampNopCount+JMP_REL32_INSTR_LENGTH); // Extra NOPs because some NOPs will be overwritten with the "JMP rel32" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel32 instruction (relative to localTrampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    write_jmp_rel32((uint8_t*)localTrampoline+retJmpOffset, (uint8_t*)injectionAddr+JMP_REL32_INSTR_LENGTH, trampNopCount);
}



// Helper function that writes bytecode for 2-byte JMP injections and the
//  local trampoline functions they utilize.
void write_bytecode_2b(void *injectionAddr, int nopCount, void *localTrampoline, int trampNopCount, void *jmpTo)
{
    // Write the injected JMP rel8 instruction:
    write_jmp_rel8(injectionAddr, localTrampoline, nopCount);

    // Create the local trampoline function:
    int retJmpOffset = write_jmp_rax_14b(localTrampoline, (void*)jmpTo, trampNopCount+JMP_REL8_INSTR_LENGTH); // Extra NOPs because some NOPs will be overwritten with the "JMP rel8" returning JMP
    
    // Obtain the write offset of the local trampoline function's returning JMP rel8 instruction (relative to localTrampoline):
    retJmpOffset += POP_RAX_INSTR_LENGTH;

    // Write the local trampoline's returning JMP rel8 instruction:
    write_jmp_rel8((uint8_t*)localTrampoline+retJmpOffset, (uint8_t*)injectionAddr+JMP_REL8_INSTR_LENGTH, trampNopCount);
}



// Writes bytecode for the series of instructions to perform an abolute JMP r64 (using JMP %rax)
//  and restore the register upon returning. Also overwrites remaining garbage bytecode with
//  the specified number of NOP instructions.
int write_jmp_rax_14b(void *writeTo, void *jmpTo, int nopCount)
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



// Writes a JMP rel8 instruction from writeTo to jmpTo, and inserts trailing NOPs if necessary
void write_jmp_rel8(void *writeTo, void *jmpTo, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)writeTo+1) = (int8_t)calculate_jmp_offset(writeTo, jmpTo, JMP_REL8_INSTR_LENGTH); // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}


// Writes a JMP rel8 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel8(void *writeTo, int8_t offset, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL8_INSTR_OPCODE; // Write opcode byte
    
    *((int8_t*)writeTo+1) = offset; // Write operand byte

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL8_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a JMP rel32 instruction from writeTo to jmpTo, and inserts trailing NOPs if necessary
void write_jmp_rel32(void *writeTo, void *jmpTo, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)writeTo+1) = (int32_t)calculate_jmp_offset(writeTo, jmpTo, JMP_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a JMP rel32 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void write_jmp_rel32(void *writeTo, int32_t offset, int nopCount)
{
    *(uint8_t*)writeTo = JMP_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)writeTo+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + JMP_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a CALL rel32 instruction from writeTo to procedure, and inserts trailing NOPs if necessary
void write_call_rel32(void *writeTo, void *procedure, int nopCount)
{
    *(uint8_t*)writeTo = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(uint32_t*)((uint8_t*)writeTo+1) = (int32_t)calculate_jmp_offset(writeTo, procedure, CALL_REL32_INSTR_LENGTH); // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a CALL rel32 instruction from writeTo using the given offset, and inserts trailing NOPs if necessary
void write_call_rel32(void *writeTo, int32_t offset, int nopCount)
{
    *(uint8_t*)writeTo = CALL_REL32_INSTR_OPCODE; // Write opcode byte
    
    *(int32_t*)((uint8_t*)writeTo+1) = offset; // Write operand bytes

    // Erase trailing garbage bytes from overwritten instruction(s) at write address:
    memset((void*)((uint8_t*)writeTo + CALL_REL32_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "near return" (RET) instruction to writeTo and inserts trailing NOPs if necessary
void write_ret(void *writeTo, int nopCount)
{
	*(uint8_t*)writeTo = RET_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "far return" (RET) instruction to writeTo and inserts trailing NOPs if necessary
void write_ret_far(void *writeTo, int nopCount)
{
	*(uint8_t*)writeTo = RET_FAR_INSTR_OPCODE; // Write instruction
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "near return, pop imm16 bytes from stack" (RET imm16) instruction to writeTo and
//  inserts trailing NOPs if necessary
void write_ret_imm16(void *writeTo, uint16_t popBytes, int nopCount)
{
	*(uint8_t*)writeTo = RET_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)writeTo+1) = popBytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Writes a "far return, pop imm16 bytes from stack" (RET imm16) instruction to writeTo and
//  inserts trailing NOPs if necessary
void write_ret_far_imm16(void *writeTo, uint16_t popBytes, int nopCount)
{
	*(uint8_t*)writeTo = RET_FAR_IMM16_INSTR_OPCODE; // Write opcode byte

	*(uint16_t*)((uint8_t*)writeTo+1) = popBytes; // Write operand bytes
	
	// Erase trailing garbage bytes from overwritten instruction at write address:
    memset((void*)((uint8_t*)writeTo + RET_IMM16_INSTR_LENGTH), NOP_INSTR_OPCODE, nopCount);
}



// Calculates the offset between a JMP rel instruction and some address:
int64_t calculate_jmp_offset(void *fromAddress, void *toAddress, int jmpInstrLength)
{
    return ((uint64_t)toAddress - (uint64_t)fromAddress - jmpInstrLength);
}

