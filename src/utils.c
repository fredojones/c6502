#include <string.h>
#include "utils.h"

/* tobstr:  convert integer n to binary representation in s with
 * a fixed width f (padded with zeroes); assume s big enough */
void tobstr(char *s, int n, int f)
{
	int i = 0;

	do {
		if (n % 2 == 0)
			s[i++] = '0';
		else
			s[i++] = '1';
	} while ((n /= 2) > 0);

	while (i < f)
		s[i++] = '0'; // pad with zeroes

	reverse(s);
	s[++i] = '\0';
}

/* reverse:  reverse string s in place */
void reverse(char *s)
{
	for (int i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		char temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
}
