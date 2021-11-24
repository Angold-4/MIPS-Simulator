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

#ifndef __DECODE_H
#define __DECODE_H

#include <stdint.h>


/* ----------------------------------------------------------------------------
	Opcode (All Instruction Types)
*/

/*
 * decode_op
 * Decode opcode from raw instruction. 
 */
int decode_opcode(uint32_t instr);

/*
 * decode_i_rt
 * Decode target register from I-type instruction. 
 */
int decode_i_rt(uint32_t instr);


/*
 * decode_r_funct
 * Decode function code from R-type instruction. 
 */
int decode_r_funct(uint32_t instr);


#endif // __DECODE_H
