#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sim.h"
#include "mips.h"
#include "shell.h"
#include "decode.h"


/* ----------------------------------------------------------------------------
	Instruction Handler Dipatch
*/
	
// function table, keyed by instruction function field
int (*FUNCTION_DISPATCH[DISPATCH_SIZE])();

// function table, keyed by instruction target field
int (*TARGET_DISPATCH[DISPATCH_SIZE])();

// function table, keyed by instruction opcode field
int (*OPCODE_DISPATCH[DISPATCH_SIZE])(); 


/* ----------------------------------------------------------------------------
	Local Prototypes 
*/

// by opcode 
int handle_j(uint32_t instr); 
int handle_jal(uint32_t instr); 
int handle_beq(uint32_t instr);
int handle_bne(uint32_t instr);
int handle_blez(uint32_t instr);
int handle_bgtz(uint32_t instr); 
int handle_addi(uint32_t instr);
int handle_addiu(uint32_t instr);
int handle_slti(uint32_t instr);
int handle_sltiu(uint32_t instr);
int handle_andi(uint32_t instr);
int handle_ori(uint32_t instr);
int handle_xori(uint32_t instr);
int handle_lui(uint32_t instr);
int handle_lb(uint32_t instr);
int handle_lh(uint32_t instr);
int handle_lw(uint32_t instr);
int handle_lbu(uint32_t instr);
int handle_lw(uint32_t instr); 
int handle_lhu(uint32_t instr);
int handle_sb(uint32_t instr);
int handle_sh(uint32_t instr);
int handle_sw(uint32_t instr);

// by function code 
int handle_bltz(uint32_t instr);
int handle_bgez(uint32_t instr);
int handle_sll(uint32_t instr); 
int handle_srl(uint32_t instr);
int handle_sra(uint32_t instr);
int handle_sllv(uint32_t instr); 
int handle_srlv(uint32_t instr);
int handle_srav(uint32_t instr);
int handle_jr(uint32_t instr);
int handle_jalr(uint32_t instr);
int handle_syscall(uint32_t instr);
int handle_mfhi(uint32_t instr);
int handle_mthi(uint32_t instr);
int handle_mflo(uint32_t instr);
int handle_mtlo(uint32_t instr); 
int handle_mult(uint32_t instr); 
int handle_multu(uint32_t instr);
int handle_div(uint32_t instr);
int handle_divu(uint32_t instr); 
int handle_add(uint32_t instr);
int handle_addu(uint32_t instr);
int handle_sub(uint32_t instr);
int handle_subu(uint32_t instr);
int handle_and(uint32_t instr);
int handle_or(uint32_t instr);
int handle_xor(uint32_t instr);
int handle_nor(uint32_t instr);
int handle_slt(uint32_t instr);
int handle_sltu(uint32_t instr);

// by target code 
int handle_bltz(uint32_t instr); 
int handle_bgez(uint32_t instr); 
int handle_bltzal(uint32_t instr); 
int handle_bgezal(uint32_t instr); 

// unrecognized codes 
int handle_unrecognized_opcode(uint32_t instr); 
int handle_unrecognized_function(uint32_t instr); 
int handle_unrecognized_target(uint32_t instr); 


/* ----------------------------------------------------------------------------
	Instruction Handler Dispatch Setup 
	execute first in shell.c (main)
*/

void init_opcode_dispatch(void) {
	memset(OPCODE_DISPATCH, 0, sizeof(void *)*DISPATCH_SIZE);

	// setup handler for unrecognized instructions
	// prevents segmentation faults on bad opcode decodes 
	for (int i = 0; i < DISPATCH_SIZE; i++) {
		OPCODE_DISPATCH[i] = handle_unrecognized_opcode;
	}

	// instruction handlers, in increasing order of opcode value
	OPCODE_DISPATCH[OPCODE_J]     = handle_j; 
	OPCODE_DISPATCH[OPCODE_JAL]   = handle_jal; 
	OPCODE_DISPATCH[OPCODE_BEQ]   = handle_beq; 
	OPCODE_DISPATCH[OPCODE_BNE]   = handle_bne; 
	OPCODE_DISPATCH[OPCODE_BLEZ]  = handle_blez; 
	OPCODE_DISPATCH[OPCODE_BGTZ]  = handle_bgtz;
	OPCODE_DISPATCH[OPCODE_ADDI]  = handle_addi;
	OPCODE_DISPATCH[OPCODE_ADDIU] = handle_addiu;
	OPCODE_DISPATCH[OPCODE_SLTI]  = handle_slti;
	OPCODE_DISPATCH[OPCODE_SLTIU] = handle_sltiu;
	OPCODE_DISPATCH[OPCODE_ANDI]  = handle_andi;
	OPCODE_DISPATCH[OPCODE_ORI]  = handle_ori; 
	OPCODE_DISPATCH[OPCODE_XORI]  = handle_xori;
	OPCODE_DISPATCH[OPCODE_LUI]   = handle_lui;
	OPCODE_DISPATCH[OPCODE_LB]    = handle_lb;
	OPCODE_DISPATCH[OPCODE_LH]    = handle_lh;
	OPCODE_DISPATCH[OPCODE_LW]    = handle_lw;
	OPCODE_DISPATCH[OPCODE_LBU]   = handle_lbu; 
	OPCODE_DISPATCH[OPCODE_LW]    = handle_lw; 
	OPCODE_DISPATCH[OPCODE_LHU]   = handle_lhu;
	OPCODE_DISPATCH[OPCODE_SB]    = handle_sb;
	OPCODE_DISPATCH[OPCODE_SH]    = handle_sh;
	OPCODE_DISPATCH[OPCODE_SW]    = handle_sw;
}

void init_function_dispatch(void) {
	memset(FUNCTION_DISPATCH, 0, sizeof(void *)*DISPATCH_SIZE);

	// setup handler for unrecognized instructions
	// prevents segmentation faults on bad function decodes 
	for (int i = 0; i < DISPATCH_SIZE; i++) {
		FUNCTION_DISPATCH[i] = handle_unrecognized_function;
	}

	// instruction handlers, in increasing order of function code value
	FUNCTION_DISPATCH[FUNC_SLL]     = handle_sll; 
	FUNCTION_DISPATCH[FUNC_SRL]     = handle_srl;
	FUNCTION_DISPATCH[FUNC_SRA]     = handle_sra;
	FUNCTION_DISPATCH[FUNC_SLLV]    = handle_sllv; 
	FUNCTION_DISPATCH[FUNC_SRLV]    = handle_srlv;
	FUNCTION_DISPATCH[FUNC_SRAV]    = handle_srav;
	FUNCTION_DISPATCH[FUNC_JR]      = handle_jr;
	FUNCTION_DISPATCH[FUNC_JALR]    = handle_jalr;
	FUNCTION_DISPATCH[FUNC_SYSCALL] = handle_syscall; 
	FUNCTION_DISPATCH[FUNC_MFHI]    = handle_mfhi;
	FUNCTION_DISPATCH[FUNC_MTHI]    = handle_mthi;
	FUNCTION_DISPATCH[FUNC_MFLO]    = handle_mflo;
	FUNCTION_DISPATCH[FUNC_MTLO]    = handle_mtlo;
	FUNCTION_DISPATCH[FUNC_MULT]    = handle_mult; 
	FUNCTION_DISPATCH[FUNC_MULTU]   = handle_multu; 
	FUNCTION_DISPATCH[FUNC_DIV]     = handle_div;
	FUNCTION_DISPATCH[FUNC_DIVU]    = handle_divu;
	FUNCTION_DISPATCH[FUNC_ADD]     = handle_add; 
	FUNCTION_DISPATCH[FUNC_ADDU]    = handle_addu;
	FUNCTION_DISPATCH[FUNC_SUB]     = handle_sub;
	FUNCTION_DISPATCH[FUNC_SUBU]    = handle_subu;
	FUNCTION_DISPATCH[FUNC_AND]     = handle_and;
	FUNCTION_DISPATCH[FUNC_OR]      = handle_or; 
	FUNCTION_DISPATCH[FUNC_XOR]     = handle_xor;
	FUNCTION_DISPATCH[FUNC_NOR]     = handle_nor; 
	FUNCTION_DISPATCH[FUNC_SLT]     = handle_slt;
	FUNCTION_DISPATCH[FUNC_SLTU]    = handle_sltu;
}

void init_target_dispatch(void) {
	memset(TARGET_DISPATCH, 0, sizeof(void *)*DISPATCH_SIZE);

	// setup handler for unrecognized instructions
	// prevents segmentation faults on bad function decodes 
	for (int i = 0; i < DISPATCH_SIZE; i++) {
		TARGET_DISPATCH[i] = handle_unrecognized_target;
	}

	// instruction handlers, in increasing order of target code value
	TARGET_DISPATCH[TARGET_BLTZ]   = handle_bltz; 
	TARGET_DISPATCH[TARGET_BGEZ]   = handle_bgez;
	TARGET_DISPATCH[TARGET_BLTZAL] = handle_bltzal;
	TARGET_DISPATCH[TARGET_BGEZAL] = handle_bgezal;
}


void process_instruction() {
    // read the instr from memory text segment
    uint32_t raw_instr = mem_read_32(CURRENT_STATE.PC);

    int op = decode_opcode(raw_instr);

    if (op == OPCODE_SPECIAL) { // 0
	// for special instructions, decode the function value from instruction
	int func = decode_r_funct(raw_instr);
	(*FUNCTION_DISPATCH[func])(raw_instr); // execute this function
    } else if (op == OPCODE_REGIMM) {
	// for regimm instructions, decode the target value from instruction
	int target = decode_i_rt(raw_instr);
	(*TARGET_DISPATCH[op])(raw_instr);
    } else {
	// otherwise, dispatch the appropriate instruction handler based on opcode
	(*OPCODE_DISPATCH[op])(raw_instr);
    }
}





/* ----------------------------------------------------------------------------
	Instruction Handlers, by Opcode 
*/

/*
 * handle_j 
 * Jump 
 * Opcode: 2
 * The 26-bit target address is shifted left two bits 
 * and combined with the high-order bits of the address of the delay slot. 
 * The program unconditionally jumps to this calculated address with a delay of one instruction.
 */

int handle_j(uint32_t instr) {
    // decode target address and shift left by 2 bits
    uint32_t target = (decode_j_target(instr) << 2); // why shift left by 2 bits?  -> Avoid overflow? (Maybe just the rule)

    // isolate high order bits of current address
    uint32_t current_addr = (CURRENT_STATE.PC & MASK_PC_HIGH); // MASK_PC_HIGH 0xF0000000

    // Update the program counter unconditionally
    NEXT_STATE.PC = current_addr + target;

    return STATUS_OK;
}




