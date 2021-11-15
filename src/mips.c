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
#include <stdint.h>
#include <string.h>

#include "sim.h"
#include "mips.h"
#include "shell.h"
#include "decode.h"

