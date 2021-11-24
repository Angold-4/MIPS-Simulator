/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   Based on specifications published                         */
/*   by Professor Onur Mutlu for his 2015 offering             */
/*   of Introduction to Computer Architecture at               */
/*   Carnegie Mellon University                                */
/*                                                             */
/*   Jiawei Wang (Angold-4) Nov 2021                           */ 
/*                                                             */
/***************************************************************/

// Instruction decode helpers

#include <stdint.h>
#include <stdlib.h>

#include "mips.h"



/* ----------------------------------------------------------------------------
 * Opcode (All Instruction Types)
 * Decode opcode from raw instruction.
 */

int decode_opcode(uint32_t instr) {
    return (int) ((instr & MASK_OPCODE)) >> SHIFT_OPCODE;
}


int decode_i_rt(uint32_t instr) {
	return (int) ((instr & MASK_I_RT) >> SHIFT_I_RT);
}

/* ----------------------------------------------------------------------------
	J-Type Instructions 
	See module header file (decode.h) for detailed function comments. 
*/


uint32_t decode_j_target(uint32_t instr) {
    //The 26-bit target address is shifted left two bits 
    //and combined with the high-order bits of the address of the delay slot. 
    //The program unconditionally jumps to this calculated address with a delay of one instruction.

    return (uint32_t) ((instr & MASK_J_TARGET) >> SHIFT_J_TARGET);  // this is target
    // SHIFT_J_TARGET = 0
    // MASK_J_TARGET = 0x03FFFFFF
    // 00000011 11111111 11111111 11111111 

}

// decode_r_funct
// Decode function code from R-type instruction. 

int decode_r_funct(uint32_t instr) {
	return (int) ((instr & MASK_R_FUNCT) >> SHIFT_R_FUNCT); 
}
