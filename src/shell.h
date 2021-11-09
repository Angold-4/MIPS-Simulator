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

#ifndef _SIM_SHELL_H_
#define _SIM_SHELL_H_

#include <stdint.h>

#define FALSE 0
#define TRUE  1

#define MIPS_REGS 32

typedef struct CPU_State_Struct {
  uint32_t PC;		         // program counter
  uint32_t REGS[MIPS_REGS];      // register file
  uint32_t HI; 			 // special register for mul/div
  uint32_t LO;                   // special register for mul/div
} CPU_State;

extern CPU_State CURRENT_STATE;
extern CPU_State NEXT_STATE;

extern int RUN_BIT;	/* run bit */

uint32_t mem_read_32  (uint32_t address);
void     mem_write_32 (uint32_t address, uint32_t value);

void process_instruction();

#endif
