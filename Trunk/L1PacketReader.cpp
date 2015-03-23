#include "StdAfx.h"

#ifdef USE_INTERNAL_ALLOCATOR
sL1PacketReader StaticL1Reader;
#endif

sL1PacketReader *InitL1PacketReader( char *FName )
{
	Dprintf( DLVerbose, "Started creating L1 packet reader" );

	if( FileExists( FName ) == 0 )
	{
		Dprintf( DLDebug, "\t PR : Could not open file %s for reading. Exiting", FName );
		return NULL;
	}

#ifdef USE_INTERNAL_ALLOCATOR
	sL1PacketReader *PR = &StaticL1Reader;
#else
	sL1PacketReader *PR = (sL1PacketReader*)EmbededMalloc( sizeof( sL1PacketReader ) );
#endif
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

//	PR->FileName = _strdup( FName );

	Dprintf( DLVerbose, "\t Finished creating L1 packet reader. All OK" );

	return PR;
}

void DestroyL1PacketReader( sL1PacketReader **PR )
{
	Dprintf( DLVerbose, "Started destroying L1 packet reader" );
	if( (*PR)->FileName )
	{
		EmbededFree( (*PR)->FileName );
		(*PR)->FileName = NULL;
	}
	if( (*PR)->File )
	{
		fclose( (*PR)->File );
		(*PR)->File = NULL;
	}
#ifndef USE_INTERNAL_ALLOCATOR
	EmbededFree( (*PR) );
#endif
	*PR = NULL;
	Dprintf( DLVerbose, "\t Finished destroying L1 packet reader" );
}

int IsValidL1Symbol( char *Line )
{
	if( Line[0] != '0' && Line[0] != '1' && Line[1] != ' ' )
		return -1;

	for( int i=0;i<L1SymbolListSize;i++)
#ifndef USE_INTERNAL_ALLOCATOR
		if( stristr( &Line[2], L1SymbolList[i]->Name ) == &Line[2] )
#else
		if( EmbededStrStr( &Line[2], L1SymbolList[i]->Name ) == &Line[2] )
#endif
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

	if( feof( PR->File ) )
	{
		Dprintf( DLVerbose, "\t Finished PR Read 1 line" );
		return 1;
	}

	strcpy_s( PR->LineBuffer, MAX_READER_LINE_BUFFER_LENGTH, LineBuffer );
	
	//replace EOL for the sake of nice formatting
	for( int i=0;i<MAX_READER_LINE_BUFFER_LENGTH && PR->LineBuffer[i] != 0; i++ )
		if( PR->LineBuffer[i] == '\n' || PR->LineBuffer[i] == '\r' )
			PR->LineBuffer[i] = 0;

	Dprintf( DLVerbose, "\t Finished PR Read 1 line" );

	return 0;
}

int L1PacketReaderProcessFile( sL1PacketReader *PR, sL0PacketWriter *PW )
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
			EmbededFree( OutData );
		}
	}
	Dprintf( DLVerbose, "\t Finished PR process whole file" );
	return 0;
}
