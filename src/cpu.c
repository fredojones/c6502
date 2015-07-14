#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "utils.h"


/* cpurun:  execute next logical set of instructions; for multi-byte
 * instructions, this will increment the PC multiple times */
void cpurun(cpu_state *cpu)
{
	uint8_t op = cpu->memory[cpu->pc];
	uint8_t op2, op3;

	switch (op) {
		// ADC
		case 0x69:	// Immediate
			op2 = nextbyte(cpu);
			adc(cpu, op2);
			break;
		case 0x65:	// Zero Page
			op2 = cpu->memory[nextbyte(cpu)];
			adc(cpu, op2);
			break;
		case 0x75:	// Zero Page,X
			op2 = cpu->memory[nextbyte(cpu) + cpu->x];
			adc(cpu, op2);
			break;
		case 0x6D:	// Absolute
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			adc(cpu, cpu->memory[absd(op2, op3)]);
			break;
		case 0x7D:	// Absolute,X
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			adc(cpu, cpu->memory[absx(cpu, op2, op3)]);
			break;
		case 0x79:	// Absolute,Y
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			adc(cpu, cpu->memory[absy(cpu, op2, op3)]);
			break;
		case 0x61:	// Indirect,X
			op2 = nextbyte(cpu);
			adc(cpu, cpu->memory[indx(cpu, op2)]);
			break;
		case 0x71:	// Indirect,Y
			op2 = nextbyte(cpu);
			adc(cpu, cpu->memory[indy(cpu, op2)]);
			break;

		// LDA
		case 0xA9:	// Immediate
			op2 = nextbyte(cpu);
			cpu->a = op2;
			if (op2 == 0)
				cpu->flags ^= 0x2;
			break;
		// LDX
		case 0xA2:	// Immediate
			op2 = nextbyte(cpu);
			cpu->x = op2;
			if (op2 == 0)
				cpu->flags ^= 0x2;
			break;
		// LDY
		case 0xA0:	// Immediate
			op2 = nextbyte(cpu);
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
			op2 = nextbyte(cpu);
			cpu->memory[op2] = cpu->a;
			break;
		case 0x95:	// Zero Page,X
			op2 = nextbyte(cpu);
			cpu->memory[op2 + cpu->x] = cpu->a;
			break;
		case 0x8D:	// Absolute
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			cpu->memory[absd(op2, op3)] = cpu->a;
			break;
		case 0x9D:	// Absolute,X
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			cpu->memory[absx(cpu, op2, op3)] = cpu->a;
			break;
		case 0x99:	// Absolute,Y
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			cpu->memory[absy(cpu, op2, op3)] = cpu->a;
			break;
		case 0x81:	// Indirect,X
			op2 = nextbyte(cpu);
			cpu->memory[indx(cpu, op2)] = cpu->a;
			break;
		case 0x91:	// Indirect,Y
			op2 = nextbyte(cpu);
			cpu->memory[indy(cpu, op2)] = cpu->a;
			break;

		// STX
		case 0x86:	// Zero Page
			op2 = nextbyte(cpu);
			cpu->memory[op2] = cpu->x;
			break;
		case 0x96:	// Zero Page,Y
			op2 = nextbyte(cpu);
			cpu->memory[op2 + cpu->y] = cpu->x;
			break;
		case 0x8E:	// Absolute
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			cpu->memory[absd(op2, op3)] = cpu->x;
			break;

		// STY
		case 0x84:	// Zero Page
			op2 = nextbyte(cpu);
			cpu->memory[op2] = cpu->y;
			break;
		case 0x94:	// Zero Page,X
			op2 = nextbyte(cpu);
			cpu->memory[op2 + cpu->x] = cpu->y;
			break;
		case 0x8C:	// Absolute
			op2 = nextbyte(cpu), op3 = nextbyte(cpu);
			cpu->memory[absd(op2, op3)] = cpu->y;
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


/* adc:  add operand to accumulator, setting carry flag if addition overflows;
 * and sign flag if result is negative */
void adc(cpu_state *cpu, uint8_t operand)
{
	uint16_t res;
	// set carry bit if we overflow uint8_t
	if ((res = cpu->a + operand) > 0xFF) {
		res -= 0x100;
		cpu->flags |= 0x1;
	}
	else
		cpu->flags &= ~0x1;

	// set sign bit if the result is negative
	if (res > 0x7F)
		cpu->flags |= 0x80;
	else
		cpu->flags &= ~0x80;

	cpu->a = res;
}


/* Absolute address */
uint16_t absd(uint8_t lsb, uint8_t msb)
{
	return litend(lsb, msb);
}

/* Absolute,X; will be absolute + X register */
uint16_t absx(cpu_state *cpu, uint8_t lsb, uint8_t msb)
{
	return litend(lsb, msb) + cpu->x;
}

/* Absolute,X; will be absolute + Y register */
uint16_t absy(cpu_state *cpu, uint8_t lsb, uint8_t msb)
{
	return litend(lsb, msb) + cpu->y;
}

/* Indirect,X: address will be formed of LSB byte at zero page address (op + cpu->x)
 * and MSB byte at (op + cpu->x + 1) */
uint16_t indx(cpu_state *cpu, uint8_t op)
{
	uint8_t op2 = cpu->memory[op + cpu->x];		// LSB
	uint8_t op3 = cpu->memory[op + cpu->x + 1];	// MSB
	return litend(op2, op3);
}

/* Indirect,Y: address will be formed of (LSB byte at zero page address (op)
 * and MSB byte at (op + 1)) with Y register added to final address */
uint16_t indy(cpu_state *cpu, uint8_t op)
{
	uint8_t op2 = cpu->memory[op];				// LSB
	uint8_t op3 = cpu->memory[op + 1];			// MSB
	return litend(op2, op3) + cpu->y;
}
