#ifndef UTILS_H
#define UTILS_H

/* convert LSB (op1) and MSB (op2) to little endian int */
#define litend(op1, op2) (((op2) << 8) | (op1))

void tobstr(char *s, int n, int f);
void reverse(char *s);

#endif /* UTILS_H */
