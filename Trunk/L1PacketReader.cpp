#include "StdAfx.h"

sL1PacketReader *InitL1PacketReader( char *FName )
{
	Dprintf( DLVerbose, "Started creating L1 packet reader" );

	if( FileExists( FName ) == 0 )
	{
		Dprintf( DLDebug, "\t PR : Could not open file %s for reading. Exiting", FName );
		return NULL;
	}

	sL1PacketReader *PR = (sL1PacketReader*)malloc( sizeof( sL1PacketReader ) );
	if( PR == NULL )
	{
		return NULL;
	}
	memset( PR, 0, sizeof( sL1PacketReader ) );

	errno_t er = fopen_s( &PR->File, FName, "rt" );
	if( PR->File == NULL )
	{
		Dprintf( DLDebug, "\t PR : Could not open %s file for read access. Error code %d. Exiting", FName, er );
		return NULL;
	}

	PR->FileName = _strdup( FName );

	Dprintf( DLVerbose, "\t Finished creating L1 packet reader. All OK" );

	return PR;
}

void DestroyL1PacketReader( sL1PacketReader **PR )
{
	Dprintf( DLVerbose, "Started destroying L1 packet reader" );
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
	Dprintf( DLVerbose, "\t Finished destroying L1 packet reader" );
}

int IsValidL1Symbol( char *Line )
{
	if( Line[0] != '0' && Line[0] != '1' && Line[1] != ' ' )
		return -1;

	for( int i=0;i<L1SymbolListSize;i++)
		if( stristr( &Line[2], L1SymbolList[i]->Name ) )
			return i;

	return -1;
}

int ReadNextLine( sL1PacketReader *PR )
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
		strcpy_s( PR->LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, "" );
		return 2;
	}

	char LineBuffer[MAX_READER_LINE_BUFFER_LENGTH];
	do {
		char *ret = fgets( LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, PR->File );
		PR->LineCounter++;
	}
	while( IsValidL1Symbol( LineBuffer ) == -1 && !feof( PR->File ) );

	memset( PR->LineBuffer, 0, MAX_READER_LINE_BUFFER_LENGTH );	//just in case shit happens, we can be sure to not read from previous line data
	strcpy_s( PR->LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, LineBuffer );

	Dprintf( DLVerbose, "\t Finished PR Read 1 line" );

	if( feof( PR->File ) )
		return 1;

	return 0;
}

int ProcessFile( sL1PacketReader *PR, sL0PacketWriter *PW )
{
	Dprintf( DLVerbose, "Started PR process whole file" );
	while( ReadNextLine( PR ) == 0 )
	{
		Dprintf( DLVerbose, "\t PR read line at %d : %s", PR->LineCounter, PR->LineBuffer );

		//try to build a packet that packet writer can write to file
		BYTE	*OutData = NULL;
		int		OutDataLen = 0;
		int		SymbolDefIndex = IsValidL1Symbol( PR->LineBuffer );
		L1SymbolList[SymbolDefIndex]->PacketBuilder( &OutData, &OutDataLen, PR->LineBuffer );

		if( OutDataLen > 0 )
		{
			L1L0ProcessLine( PW, OutData, OutDataLen );
			free( OutData );
		}
	}
	Dprintf( DLVerbose, "\t Finished PR process whole file" );
	return 0;
}
