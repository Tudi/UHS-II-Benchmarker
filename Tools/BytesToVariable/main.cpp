#include <stdio.h>
#include <stdlib.h>

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
		printf( "Converts a bytestream file into Microblaze write buffer friendly format\n");
		printf( "usage : BytesToVariable [InFile] [OutFile]\n");
		printf( "ex1 : BytesToVariable.exe L0_Sample_out_flipped.H2D SetupSend.h \n");
		return 0;
	}
	else if( argc < 3 )
	{
		printf( "Not enough arguments. Exiting.\n");
		return 1;
	}

	FILE *In,*Out;
	In = fopen( argv[1], "rb" );
	if( In == NULL )
	{
		printf( "Could not open input file\n");
		return 1;
	}
	Out = fopen( argv[2], "wt" );
	if( In == NULL )
	{
		printf( "Could not open output file\n");
		fclose( In );
		return 1;
	}

	fprintf( Out, "for( i = 0; i < 1024; i++ )\n" );
	fprintf( Out, "\tcfg[i] = 0x04585634;\n" );

	unsigned int ReadBuffPrev, ReadBuff, ReadCount;
	unsigned int WriteAtIndex = 0;
	unsigned int WriteCount = 0;
//	ReadCount = fread( &ReadBuffPrev, 1, sizeof( int ), In );
	ReadCount = fread( &ReadBuff, 1, sizeof( int ), In );
	while( ReadCount > 0 )
	{
		fprintf( Out, "data[%d]=0x%08X;\n", WriteAtIndex, ReadBuff );
		fprintf( Out, "count[%d]=1;\n", WriteAtIndex );
		ReadCount = fread( &ReadBuff, 1, sizeof( int ), In );
		WriteAtIndex++;
	}
	fclose( In );
	fclose( Out );
}