#include <stdio.h>
#include <stdlib.h>
#include "BitStream.h"

unsigned __int64 FlipBits( unsigned __int64 In, unsigned int BlockLenght )
{
	unsigned int ret = 0;
	for( unsigned int i=0;i<BlockLenght;i++)
	{
		ret = ret * 2 + (In & 1);
		In = In / 2;
	}
	return ret;
}

int main( int argc, char *argv[] )
{
	if( argc == 0 )
	{
		printf( "Flip bits in a file\n");
		printf( "usage : FlipBits [DataBlockLenght] [InFile] [OutFile]\n");
		printf( "ex1 : FlipBits.exe 32 L0_Sample_out.H2D L0_Sample_out_flipped.H2D \n");
		return 0;
	}
	else if( argc < 4 )
	{
		printf( "Not enough arguments. Exiting.\n");
		return 1;
	}

	DWORD DataBlockLength = atoi( argv[1] );
	if( DataBlockLength > 32 )
	{
		printf("Max 32 bits are supported for flipping\n" );
		return 1;
	}

	CBitStream In,Out;
	In.LoadStream( argv[2] );
	if( In.GetStreamLength() == 0 )
	{
		printf("Can not open input file\n");
		return 1;
	}

	DWORD ReadCounter = 0;
	while( In.GetCurrentPosition() < In.GetStreamLength() )
	{
		DWORD buff; 
		In.ReadBits( buff, DataBlockLength );
		buff = FlipBits( buff, DataBlockLength );
		Out.WriteBits( buff, DataBlockLength );
		ReadCounter++;
	}

	Out.SaveStream( argv[3] );
}