#include "StdAfx.h"

#ifdef USE_INTERNAL_ALLOCATOR
sL1PacketWriter StaticL1Writer;
#endif

sL1PacketWriter *InitL1PacketWriter( char *FName, int pLaneNr, int pHostID, int pDeviceID )
{
	Dprintf( DLVerbose, "Started creating L1 packet writer" );

#ifdef USE_INTERNAL_ALLOCATOR
	sL1PacketWriter *PW = &StaticL1Writer;
#else
	sL1PacketWriter *PW = (sL1PacketWriter*)EmbededMalloc( sizeof( sL1PacketWriter ) );
#endif
	if( PW == NULL )
	{
		return NULL;
	}
	memset( PW, 0, sizeof( sL1PacketWriter ) );

	errno_t er = fopen_s( &PW->File, FName, "wt" );
	if( PW->File == NULL )
	{
		Dprintf( DLDebug, "\t PW : Could not open %s file for write access. Error code %d. Exiting", FName, er );
		return NULL;
	}

//	PW->FileName = _strdup( FName );

	PW->Lane = pLaneNr;
	PW->HostID = pHostID;
	PW->DeviceID = pDeviceID;

	Dprintf( DLVerbose, "\t Finished creating L1 packet writer. All OK" );

	return PW;
}

void DestroyL1PacketWriter( sL1PacketWriter **PW )
{
	Dprintf( DLVerbose, "Started destroying L1 packet writer" );
	if( (*PW)->FileName )
	{
		EmbededFree( (*PW)->FileName );
		(*PW)->FileName = NULL;
	}
	if( (*PW)->File )
	{
		fclose( (*PW)->File );
		(*PW)->File = NULL;
	}
#ifndef USE_INTERNAL_ALLOCATOR
	EmbededFree( (*PW) );
#endif
	*PW = NULL;
	Dprintf( DLVerbose, "\t Finished destroying L1 packet writer" );
}

int L0L1ProcessPacket( sL1PacketWriter *PW, const char *Line )
{
	Dprintf( DLVerbose, "Started PW process 1 packet" );
	if( PW == NULL )
	{
		Dprintf( DLVerbose, "\tPW : No valid reader specified" );
		return 1;
	}
	if( PW->File == NULL )
	{
		Dprintf( DLVerbose, "\tPW : File is not open for writing" );
		return 1;
	}

	fprintf( PW->File, "%s\n", Line );

//	size_t written = fwrite( Data, 1, DataLen, PW->File );

	Dprintf( DLVerbose, "\t Finished PW process 1 packet" );
	return 0;
}