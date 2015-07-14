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

#endif /* CPU_H */
