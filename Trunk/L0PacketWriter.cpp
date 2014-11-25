#include "StdAfx.h"

sL0PacketWriter *InitL0PacketWriter( char *FName )
{
	Dprintf( DLVerbose, "Started creating L0 packet writer" );

	sL0PacketWriter *PW = (sL0PacketWriter*)malloc( sizeof( sL0PacketWriter ) );
	if( PW == NULL )
	{
		return NULL;
	}
	memset( PW, 0, sizeof( sL0PacketWriter ) );

	errno_t er = fopen_s( &PW->File, FName, "wb" );
	if( PW->File == NULL )
	{
		Dprintf( DLDebug, "\t PW : Could not open %s file for write access. Error code %d. Exiting", FName, er );
		return NULL;
	}

	PW->FileName = _strdup( FName );

	Dprintf( DLVerbose, "\t Finished creating L0 packet writer. All OK" );

	return PW;
}

void DestroyL0PacketWriter( sL0PacketWriter **PW )
{
	if( (*PW)->FileName )
	{
		free( (*PW)->FileName );
		(*PW)->FileName = NULL;
	}
	if( (*PW)->File )
	{
		fclose( (*PW)->File );
		(*PW)->File = NULL;
	}
	free( (*PW) );
	*PW = NULL;
}
