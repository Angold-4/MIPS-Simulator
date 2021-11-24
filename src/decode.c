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


// decode_r_funct
// Decode function code from R-type instruction. 

int decode_r_funct(uint32_t instr) {
	return (int) ((instr & MASK_R_FUNCT) >> SHIFT_R_FUNCT); 
}
