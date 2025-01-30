#include <stdio.h>

int	main(void)
{
	unsigned int	c;
	unsigned int	sayi;
	unsigned int	i;

	sayi = 'a';
	c = 0;
	i = 0;
	while (i < 32)
	{
		if (sayi >> i & 1)
		{
			c += 1 << i;
		}
		else
		{
			c += 0 << i;
		}
		i++;
	}
	write(1, &c, 1);
}
