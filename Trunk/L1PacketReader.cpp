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
