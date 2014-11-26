#include "StdAfx.h"

#include "StdAfx.h"

sL0PacketReader *InitL0PacketReader( char *FName )
{
	Dprintf( DLVerbose, "Started creating L0 packet reader" );

	if( FileExists( FName ) == 0 )
	{
		Dprintf( DLDebug, "\t PR : Could not open file %s for reading. Exiting", FName );
		return NULL;
	}

	sL0PacketReader *PR = (sL0PacketReader*)malloc( sizeof( sL0PacketReader ) );
	if( PR == NULL )
	{
		return NULL;
	}
	memset( PR, 0, sizeof( sL0PacketReader ) );

	errno_t er = fopen_s( &PR->File, FName, "rt" );
	if( PR->File == NULL )
	{
		Dprintf( DLDebug, "\t PR : Could not open %s file for read access. Error code %d. Exiting", FName, er );
		return NULL;
	}

	PR->FileName = _strdup( FName );

	Dprintf( DLVerbose, "\t Finished creating L0 packet reader. All OK" );

	return PR;
}

void DestroyL0PacketReader( sL0PacketReader **PR )
{
	Dprintf( DLVerbose, "Started destroying L0 packet reader" );
	if( (*PR)->FileName )
	{
		free( (*PR)->FileName );
		(*PR)->FileName = NULL;
	}
	if( (*PR)->File )
	{
		fclose( (*PR)->File );
		(*PR)->File = NULL;
	}
	free( (*PR) );
	*PR = NULL;
	Dprintf( DLVerbose, "\t Finished destroying L0 packet reader" );
}

int ReadNextLine( sL0PacketReader *PR )
{
	Dprintf( DLVerbose, "Started PR Read 1 line" );
	if( PR == NULL )
	{
		Dprintf( DLVerbose, "\tPR : No valid reader specified" );
		return 1;
	}
	if( PR->File == NULL )
	{
		Dprintf( DLVerbose, "\tPR : File is not open for reading" );
		return 1;
	}
	if( feof( PR->File ) )
	{
		Dprintf( DLVerbose, "\tPR : End of file reached" );
		memset( PR->PacketBuffer, 0, MAX_PACKET_SIZE );
		return 2;
	}

/*	char LineBuffer[MAX_READER_LINE_BUFFER_LENGTH];
	do {
		char *ret = fgets( LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, PR->File );
		PR->LineCounter++;
	}
	while( IsValidL0Symbol( LineBuffer ) == -1 && !feof( PR->File ) );

	memset( PR->LineBuffer, 0, MAX_READER_LINE_BUFFER_LENGTH );	//just in case shit happens, we can be sure to not read from previous line data
	strcpy_s( PR->LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, LineBuffer ); */

	Dprintf( DLVerbose, "\t Finished PR Read 1 line" );

	if( feof( PR->File ) )
		return 1;

	return 0;
}

int L0PacketReaderProcessFile( sL0PacketReader *PR, sL1PacketWriter *PW )
{
	Dprintf( DLVerbose, "Started PR process whole file" );
	while( ReadNextLine( PR ) == 0 )
	{
		Dprintf( DLVerbose, "\t PR read packet at stamp %d, at buffer index 0x%04X, packet Nr %04d", PR->StreamBytesRead, PR->StreamBytesRead, PR->PacketCounter );

		//try to build a packet that packet writer can write to file
/*		BYTE	*OutData = NULL;
		int		OutDataLen = 0;
		int		SymbolDefIndex = IsValidL0Symbol( PR->LineBuffer );
		L0SymbolList[SymbolDefIndex]->PacketBuilder( &OutData, &OutDataLen, PR->LineBuffer );

		if( OutDataLen > 0 )
		{
			L0L0ProcessLine( PW, OutData, OutDataLen );
			free( OutData );
		} */
	}
	Dprintf( DLVerbose, "\t Finished PR process whole file" );
	return 0;
}
