
#include <xeroslib.h>


static	unsigned int	randx = 1;

void srand(unsigned int x)
{
	randx = x;
}

int rand()
{
	return(((randx = randx*1103515245 + 12345)>>16) & 077777);
}
