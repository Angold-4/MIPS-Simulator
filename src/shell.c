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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "shell.h"
#include "mips.h"


/***************************************************************/
/* Main memory.                                                */
/***************************************************************/

#define MEM_DATA_START  0x10000000
#define MEM_DATA_SIZE   0x00100000
#define MEM_TEXT_START  0x00400000
#define MEM_TEXT_SIZE   0x00100000
#define MEM_STACK_START 0x7ff00000
#define MEM_STACK_SIZE  0x00100000
#define MEM_KDATA_START 0x90000000
#define MEM_KDATA_SIZE  0x00100000
#define MEM_KTEXT_START 0x80000000
#define MEM_KTEXT_SIZE  0x00100000

typedef struct {
    uint32_t start, size;
    uint8_t *mem; // data
} mem_region_t;

/* memory will be dynamically allocated at initialization */
mem_region_t MEM_REGIONS[] = {
    { MEM_TEXT_START,  MEM_TEXT_SIZE,  NULL },
    { MEM_DATA_START,  MEM_DATA_SIZE,  NULL },
    { MEM_STACK_START, MEM_STACK_SIZE, NULL },
    { MEM_KDATA_START, MEM_KDATA_SIZE, NULL },
    { MEM_KTEXT_START, MEM_KTEXT_SIZE, NULL }
};

#define MEM_NREGIONS (sizeof(MEM_REGIONS) / sizeof(mem_region_t))



/***************************************************************/
/*                                                             */
/* Procedure: mem_write_32                                     */
/*                                                             */
/* Purpose: Write a 32-bit word to memory                      */
/*                                                             */
/***************************************************************/

void mem_write_32(uint32_t address, uint32_t value) {
    int i;

    for (i = 0; i < MEM_NREGIONS; i++) {
	// search each segements of the memory
	if (address >= MEM_REGIONS[i].start && address < (MEM_REGIONS[i].start + MEM_REGIONS[i].size)) {
	    uint32_t offset = address - MEM_REGIONS[i].start;

	    MEM_REGIONS[i].mem[offset+3] = (value >> 24) & 0xFF; // top 8 bits
	    MEM_REGIONS[i].mem[offset+2] = (value >> 16) & 0xFF;
	    MEM_REGIONS[i].mem[offset+1] = (value >> 8) & 0xFF;
	    MEM_REGIONS[i].mem[offset+0] = (value >> 0) & 0xFF;
	    return;
	}
    }
}


/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/

void load_program(char *program_filename) {
    FILE *prog;
    int ii, word;

    // open the program file
    prog = fopen(program_filename, "r");
    if (prog == NULL) {
	printf("Error: Can't open program file %s\n", program_filename);
	exit(-1);
    }

    // read in the program
    ii = 0;
    while (fscanf(prog, "%x\n", &word) != EOF) { // stop reading when meet a white space (or EOF)
	// write 32 bits (4 words), which is one MIPS instruction
	
	mem_write_32(MEM_TEXT_START + ii, word);
	ii += 4;
    }

    CURRENT_STATE.PC = MEM_TEXT_START;

    printf("Read %d words from program into memory.\n\n", ii/4);
}


/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Allocate and zero memoryy                       */
/*                                                             */
/***************************************************************/

void init_memory() {
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
	MEM_REGIONS[i].mem = malloc(MEM_REGIONS[i].size); // allocate a RAM Space
	mem_set(MEM_REGIONS[i].mem, 0, MEM_REGIONS[i].size);
    }

}


/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/

void initialize(char *program_filename, int num_prog_files) {
    int i;

    init_memory();
    for (i = 0; i < num_prog_files; i++) {
	// bug, only execute the last file (if there are multiple input files)
	load_program(program_filename);
	while (*program_filename++ != '\0'); // safe checking
    }

    NEXT_STATE = CURRENT_STATE;
    RUN_BIT    = TRUE;
}


/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/

int main(int argc, char *argv[]) {
    FILE *dumpsim_file;

    if (argc < 2) {
	printf("Error: usage: %s <program_file_1> <program_file_2> ... \n", argv[0]);
	exit(1);
    }

    printf("MIPS Simulator\n\n");

    initialize(argv[1], argc-1);
    
    // initialize opcode and function dispatchers
}


