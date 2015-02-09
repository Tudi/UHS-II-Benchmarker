#ifndef _MEMORY_TOOLS_H_
#define _MEMORY_TOOLS_H_

void 	ResetMemoryToZero();
void	Sleep( int Milliseconds );
/*
Flip Bits. Can specify the length of data block
*/
unsigned int FlipBits( unsigned int In, unsigned int BlockLenght);
#endif
