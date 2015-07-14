#include <stdint.h>
#include <stdio.h>
#include "cpu.h"

/* convert LSB (op1) and MSB (op2) to little endian int */
#define litend(op1, op2) (((op2) << 8) | (op1))

/* cpurun:  execute next logical set of instructions; for multi-byte
 * instructions, this will increment the PC multiple times */
void cpurun(cpu_state *cpu)
{
	uint8_t op = cpu->memory[cpu->pc];
	uint8_t op2, op3;
	uint16_t res;

	switch (op) {
		// ADC
		case 0x69:	// Immediate
			// set carry bit if we overflow uint8_t
			if ((res = cpu->a + cpu->memory[++(cpu->pc)]) > 0xFF) {
				res -= 0x100;
				cpu->c = 1;
			}
			else
				cpu->c = 0;

			cpu->a = res;
			break;

		// LDA
		case 0xA9:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->a = op2;
			break;
		// LDX
		case 0xA2:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->x = op2;
			break;
		// LDY
		case 0xA0:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->y = op2;
			break;

		// TAX
		case 0xAA:
			cpu->x = cpu->a;
			break;
		// TXA
		case 0x8A:
			cpu->a = cpu->x;
			break;
		// DEX
		case 0xCA:
			cpu->x--;
			break;
		// INX
		case 0xE8:
			cpu->x++;
			break;
		// TAY
		case 0xA8:
			cpu->y = cpu->a;
			break;
		// TYA
		case 0x98:
			cpu->a = cpu->y;
			break;
		// DEY
		case 0x88:
			cpu->y--;
			break;
		// INY
		case 0xC8:
			cpu->y++;
			break;

		// STA
		case 0x85:	// Zero Page
			break;
		case 0x8D:	// Absolute
			op2 = cpu->memory[++(cpu->pc)];
			op3 = cpu->memory[++(cpu->pc)];

			cpu->memory[litend(op2, op3)] = cpu->a;
			break;

		// CLC
		case 0x18:
			cpu->c = 0;
			break;
		// SEC
		case 0x38:
			cpu->c = 1;
			break;
		// CLI
		case 0x58:
			cpu->i = 0;
			break;
		// SEI
		case 0x78:
			cpu->i = 1;
			break;
		// CLV
		case 0xB8:
			cpu->o = 0;
			break;
		// CLD
		case 0xD8:
			cpu->d = 0;
			break;
		// SED
		case 0xF8:
			cpu->d = 1;
			break;

		// BRK
		case 0x00:
			puts("Hit BRK\n");
			break;
	}

	// increment program counter
	cpu->pc++;
}

