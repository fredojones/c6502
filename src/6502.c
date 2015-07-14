#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"

#define MAXINPUT	1000	/* max input length read */


int main(int argc, char *argv[])
{
	int object_mode = 0;	/* parse file as raw byte stream or hex byte strings */
	int debug_mode = 0;		/* print register info each instruction */

	char *filepath = NULL;	/* filepath of object file */
	FILE *fp;				/* pointer to object file */

	cpu_state *cpu = malloc(sizeof(cpu_state));

	/* parse command line arguments */
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-f")) {
			if (i == argc - 1) {
				fputs("error:  -f requires filepath argument\n", stderr);
				exit(EXIT_FAILURE);
			} else {
				filepath = argv[++i];
			}
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
		char *ins;			/* character string in hex representing opcode */
		uint8_t op;			/* integer opcode */
		//int bytes = 0;		/* number of bytes read */

		while (fscanf(fp, "%s ", ins) != EOF) {
			op = (uint8_t) strtol(ins, NULL, 16);

			// Read each byte into RAM
			cpu->memory[cpu->pc++] = op;

			//bytes++;
		}

		// Reset program counter
		cpu->pc = 0;

		// Execute program
		while (cpu->pc < 0xFFFF) {
			/* if in debug mode, step through next logical instruction */
			if (debug_mode) {
				printf("A:%02X\nX:%02X\nY:%02X\nP:%02X\nS:%02X\nPC:%04X\n\n",
						cpu->a, cpu->x, cpu->y, cpu->p, cpu->s, cpu->pc);

				char *addr = malloc(MAXINPUT);
				uint16_t addrp;

				fgets(addr, MAXINPUT, stdin);
				// display memory address at location specified
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
