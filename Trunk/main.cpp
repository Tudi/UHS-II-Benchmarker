#include "StdAfx.h"

enum eValidExeModes
{
	EXE_MODE_H2D_L0	=	1,
	EXE_MODE_D2H_L0	=	2,
};

int IsValidMode( char *mode )
{
	if( stristr( mode, "h2d" ) == mode )
		return EXE_MODE_H2D_L0;
	return 0;
}

int main( int argc, char *argv[] )
{
	if( argc == 0 )
	{
		printf( "UHS II protocol benchmarker\n");
		printf( "usage : UHSIIBM [mode] [InFile] [OutFile]\n");
		printf( "available modes : \n");
		printf( "\t h2d0 - Host to device, packetize Link Layer to Physical Layer\n");
		printf( "\t d2h0 - Device to Host, Unpack Physical Layer packet to Link Layer commands\n");
		return 0;
	}
	else if( argc < 4 )
	{
		printf( "Not enough arguments. Exiting.\n");
		return 0;
	}
	if( IsValidMode( argv[1] ) == 0 )
	{
		printf( "Selected mode %s is not valid. Exiting.\n", argv[1]);
		return 0;
	}

	InitL1SymbolList();

	if( IsValidMode( argv[1] ) == EXE_MODE_H2D_L0 )
	{
		Dprintf( DLVerbose, "Started exe mode H2D" );

		sL1PacketReader *PR = InitL1PacketReader( argv[2] );
		sL0PacketWriter *PW = InitL0PacketWriter( argv[3], 0, 1, 0 );

		ProcessFile( PR, PW );

		DestroyL1PacketReader( &PR );
		DestroyL0PacketWriter( &PW );
		Dprintf( DLVerbose, "\t Finished exe mode H2D" );
	}
	return 0;
}