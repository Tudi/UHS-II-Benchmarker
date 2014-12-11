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
	if( stristr( mode, "d2h" ) == mode )
		return EXE_MODE_D2H_L0;
	return 0;
}

int main( int argc, char *argv[] )
{
	//scramble test from doc
	BYTE data[10];
	data[0] = 0xB1;
	data[1] = 0x00;
	data[2] = 0xAA;
	data[3] = 0xBB;
	data[4] = 0xCC;
	data[5] = PAD;
	*(unsigned short*)&data[6] = CRC_LSB_SWAP( crc16_ccitt( data, 6 ) );
	if( data[6] != 0xFE || data[7] != 0x1E )
		printf( "CRC generation and write failed!\n");
	ScramblePacket( data, 8 );
	if( data[0] != 0x4E || data[1] != 0x17 || data[2] != 0xC0 || data[3] != 0x14 || data[4] != 0xB2 || data[5] != PAD || data[6] != 0xFE || data[7] != 0x1E )
		printf( "Scrambler is not working as expected!!! \n" );

	if( argc == 0 )
	{
		printf( "UHS II protocol benchmarker\n");
		printf( "usage : UHSIIBM [mode] [InFile] [OutFile]\n");
		printf( "available modes : \n");
		printf( "\t h2d0 - Host to device, packetize Link Layer to Physical Layer\n");
		printf( "\t d2h0 - Device to Host, Unpack Physical Layer packet to Link Layer commands\n");
		printf( "ex1 : UHSIIBM.exe h2d0 L1_Sample.H2D L0_Sample_out.H2D \n");
		printf( "ex2 : UHSIIBM.exe d2H0 L0_Sample_out.H2D L1_Sample_out.H2D \n");
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

	// h2d0 L1_Sample.H2D L0_Sample_out.H2D
	if( IsValidMode( argv[1] ) == EXE_MODE_H2D_L0 )
	{
		Dprintf( DLVerbose, "Started exe mode H2D" );

		sL1PacketReader *PR = InitL1PacketReader( argv[2] );
		sL0PacketWriter *PW = InitL0PacketWriter( argv[3], 0, 1, 0 );

		L1PacketReaderProcessFile( PR, PW );

		DestroyL1PacketReader( &PR );
		DestroyL0PacketWriter( &PW );

		Dprintf( DLVerbose, "\t Finished exe mode H2D" );
	}
	// d2H0 L0_Sample_out.H2D L1_Sample_out.H2D
	else if( IsValidMode( argv[1] ) == EXE_MODE_D2H_L0 )
	{
		Dprintf( DLVerbose, "Started exe mode D2H" );

		sL0PacketReader *PR = InitL0PacketReader( argv[2] );
		sL1PacketWriter *PW = InitL1PacketWriter( argv[3], 0, 1, 0 );

		L0PacketReaderProcessFile( PR, PW );

		DestroyL0PacketReader( &PR );
		DestroyL1PacketWriter( &PW );

		Dprintf( DLVerbose, "\t Finished exe mode D2H" );
	}
	return 0;
}