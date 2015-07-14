#include <stdint.h>
#include <stdio.h>
#include "cpu.h"

/* cpurun:  execute next logical set of instructions; for multi-byte
 * instructions, this will increment the PC multiple times */
void cpurun(cpu_state *cpu)
{
	uint8_t op = cpu->memory[cpu->pc];
	uint8_t op2, op3;

	switch (op) {
		// LDA
		case 0xA9:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->a = op2;
			break;

		// STA
		case 0x85:	// Zero Page
			break;
		case 0x8D:	// Absolute
			op2 = cpu->memory[++(cpu->pc)];
			op3 = cpu->memory[++(cpu->pc)];

			// convert from little endian
			cpu->memory[(op3 << 8) | op2] = cpu->a;
			break;
	}

	// increment program counter
	cpu->pc++;
}
