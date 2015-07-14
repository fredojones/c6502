#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"
#include "utils.h"

#define MAXINPUT	1000	/* max input length read */
#define PCINIT		0x0600	/* where the program will begin in memory */


int main(int argc, char *argv[])
{
	int object_mode = 0;	/* parse file as raw byte stream or hex byte strings */
	int debug_mode = 0;		/* print register info each instruction */

	char *filepath = NULL;	/* filepath of object file */
	FILE *fp;				/* pointer to object file */

	cpu_state *cpu = malloc(sizeof(cpu_state));
	cpu->flags = 0x20;			/* set unused bit */
	cpu->s = 0xFF;				/* initialize stack pointer */
	cpu->pc = PCINIT;

	/* parse command line arguments */
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-f")) {
			if (i == argc - 1) {
				fputs("error:  -f requires filepath argument\n", stderr);
				exit(EXIT_FAILURE);
			} else
				filepath = argv[++i];
		}

		if (!strcmp(argv[i], "-d"))
			debug_mode = 1;
	}

	if (filepath == NULL)
		fp = stdin;
	else
		fp = fopen(filepath, "r");


	if (!object_mode) {
		/* parse input as series of whitespace-seperated hex byte strings */
		char *ins = malloc(20);	/* character string in hex representing opcode */
		uint8_t op;				/* integer opcode */
		//int bytes = 0;		/* number of bytes read */

		// TODO limit fscanf at 20 chars
		while (fscanf(fp, "%s ", ins) != EOF) {
			op = (uint8_t) strtol(ins, NULL, 16);

			// Read each byte into RAM
			cpu->memory[cpu->pc++] = op;

			//bytes++;
		}

		// Reset program counter
		cpu->pc = PCINIT;

		// Execute program
		for (;;) {
			/* if in debug mode, step through next logical instruction */
			if (debug_mode) {
				char *s = malloc(10);
				tobstr(s, cpu->flags, 8);

				printf("A:%02X\tX:%02X\tY:%02X\tP:%02X\tS:%02X\n"
					   "Flags:\tNV BDIZC\n"
					   "\t%8s\n"
					   "PC:%04X\n\n",
						cpu->a, cpu->x, cpu->y, cpu->p, cpu->s,
						s, cpu->pc);


				// display memory address at location specified
				char *addr = malloc(MAXINPUT);
				uint16_t addrp;

				fgets(addr, MAXINPUT, stdin);
				if (addr[0] == ':') {
					addr++;	/* skip the colon */

					// TODO: Bounds checking
					addrp = (uint16_t) strtol(addr, NULL, 16);
					printf("@ $%s: %02X\n\n", addr, cpu->memory[addrp]);
				}
			}

			/* execute the current logical instruction */
			cpurun(cpu);
		}
	}
	else {
		/* parse file as byte stream */
		//FILE *fp = fopen(filepath, "rb");
	}

	exit(EXIT_SUCCESS);
}
