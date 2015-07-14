#ifndef CPU_H
#define CPU_H

#define MEMLIMIT	0xFFFF /* 64 kilobytes of RAM */

typedef struct {
	/* registers */
	uint8_t a, x, y, p, s;
	uint16_t pc;

	/* flags */
	uint8_t flags; /* NV BDIZC */

	/* RAM */
	uint8_t memory[MEMLIMIT];
} cpu_state;

void cpurun(cpu_state *cpu);
uint8_t nextbyte(cpu_state *cpu);
void adc(cpu_state *cpu, uint8_t operand);

uint16_t absd(uint8_t lsb, uint8_t msb);
uint16_t absx(cpu_state *cpu, uint8_t lsb, uint8_t msb);
uint16_t absy(cpu_state *cpu, uint8_t lsb, uint8_t msb);
uint16_t indx(cpu_state *cpu, uint8_t op);
uint16_t indy(cpu_state *cpu, uint8_t op);

/* return next byte from RAM and increment the program counter */
#define nextbyte(cpu) ((cpu)->memory[++((cpu)->pc)])

#endif /* CPU_H */
