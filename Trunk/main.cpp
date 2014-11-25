#include "StdAfx.h"

int IsValidMode( char *mode )
{
	if( stristr( mode, "h2d" ) == mode )
		return 1;
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
	return 0;
}