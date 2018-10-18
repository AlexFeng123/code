#include <string.h>

void main()
{
	char	buf1[8];
	char	buf2[16];
	int	i;

	for (i = 0; i < 16; ++i)
		buf2[i] = 'A';
	strcpy(buf1, buf2);
}
