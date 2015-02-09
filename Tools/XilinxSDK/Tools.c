#include "StdAfx.h"

volatile unsigned read;

void ResetMemoryToZero()
{
	int i;
	for(i = 0; i<1024; i++)
	{
		data[i] = 0;
		cfg[i] = 0;
		count[i] = 0;
		read = count_read[i];
	}
}

void Sleep( int Milliseconds )
{
	volatile int i;
    for(i=0;i<10000 * Milliseconds;i++);
    i++;

}

unsigned int FlipBits( unsigned int In, unsigned int BlockLenght )
{
	unsigned int i, ret = 0;
	for( i=0;i<BlockLenght;i++)
	{
		ret = ret * 2 + (In & 1);
		In = In / 2;
	}
	return ret;
}
