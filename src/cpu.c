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
				cpu->flags |= 0x1;
			}
			else
				cpu->flags &= ~0x1;

			if (res > 0x7F)
				cpu->flags |= 0x80;
			else
				cpu->flags &= ~0x80;

			cpu->a = res;
			break;

		// LDA
		case 0xA9:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->a = op2;
			if (op2 == 0)
				cpu->flags ^= 0x2;
			break;
		// LDX
		case 0xA2:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->x = op2;
			if (op2 == 0)
				cpu->flags ^= 0x2;
			break;
		// LDY
		case 0xA0:	// Immediate
			op2 = cpu->memory[++(cpu->pc)];
			cpu->y = op2;
			if (op2 == 0)
				cpu->flags ^= 0x2;
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
			cpu->flags &= ~0x1;
			break;
		// SEC
		case 0x38:
			cpu->flags |= 0x1;
			break;
		// CLI
		case 0x58:
			cpu->flags &= ~0x4;
			break;
		// SEI
		case 0x78:
			cpu->flags |= 0x4;
			break;
		// CLV
		case 0xB8:
			cpu->flags &= ~0x40;
			break;
		// CLD
		case 0xD8:
			cpu->flags &= ~0x8;
			break;
		// SED
		case 0xF8:
			cpu->flags |= 0x8;
			break;

		// BRK
		case 0x00:
			puts("Hit BRK\n"); // for debugging
			cpu->flags |= 0x10;
			cpu->pc++;
			break;
	}

	// increment program counter
	cpu->pc++;
}

