#include "StdAfx.h"
#include <conio.h>

enum eValidExeModes
{
	EXE_MODE_H2D_L0			=	1,
	EXE_MODE_D2H_L0			=	2,
	EXE_MODE_H2D_LINEFEED	=	3,
};

int IsValidMode( char *mode )
{
	StrToUpper( mode );
	if( EmbededStrStr( mode, "H2D" ) == mode )
		return EXE_MODE_H2D_L0;
	else if( EmbededStrStr( mode, "D2H" ) == mode )
		return EXE_MODE_D2H_L0;
	else if( EmbededStrStr( mode, "LINEFEED" ) == mode )
		return EXE_MODE_H2D_LINEFEED;
	return 0;
}

int ConsoleReadLine( char *Buffer, int MaxLen )
{
	for(int i = 0; i < MaxLen; )
	{   
		int c = _getch(); 
		if( c == '\t' )
			c = ' ';
		if( c == '\r' || c == '\n' )
		{
			Buffer[i] = 0;
			printf("\n");
			return i;
		}
		else if( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c == ' ' )
			Buffer[i] = (char)c;
		else
			continue;
		printf( "%c", c );
		i++;
	}   

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
	// linefeed 0 0
	else if( IsValidMode( argv[1] ) == EXE_MODE_H2D_LINEFEED )
	{
		Dprintf( DLVerbose, "Started exe mode H2D linefeedmode" );

		char	LineBuffer[MAX_PACKET_SIZE];
		BYTE	OutData[MAX_PACKET_SIZE];
		BYTE	*OutDataPointer = &OutData[0];
		ProjectSettingOnePacketBuffer = 1;
//		int Er = scanf_s( "%s", LineBuffer, MAX_PACKET_SIZE );
		int ReadCount = ConsoleReadLine( LineBuffer, MAX_PACKET_SIZE );
		while( ReadCount > 0 )
		{
			//check for packet type
			int	SymbolDefIndex = IsValidL1SymbolEmbeded( LineBuffer );
			if( SymbolDefIndex < 0 || SymbolDefIndex > L1SymbolListSize )
			{
				printf("Did not understand line. Skipp processing it\n");
				goto PROCESS_NEXT;
			}

			int		OutDataLen = 0;

			L1SymbolList[ SymbolDefIndex ]->PacketBuilder( &OutDataPointer, &OutDataLen, LineBuffer );

			if( OutDataLen > 0 )
			{
				char FormattedPacket[ MAX_PACKET_SIZE ];
				FormatPacket( OutData, OutDataLen, FormattedPacket, MAX_PACKET_SIZE );
				printf( "%s\n", FormattedPacket );
			}
PROCESS_NEXT:
			ReadCount = ConsoleReadLine( LineBuffer, MAX_PACKET_SIZE );
		}
		Dprintf( DLVerbose, "\t Finished exe mode D2H" );
	}
	return 0;
}