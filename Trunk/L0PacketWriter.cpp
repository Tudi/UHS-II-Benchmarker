#include "StdAfx.h"

#ifdef USE_INTERNAL_ALLOCATOR
sL0PacketWriter StaticL0Writer;
#endif

sL0PacketWriter *InitL0PacketWriter( char *FName, int pLaneNr, int pHostID, int pDeviceID )
{
	Dprintf( DLVerbose, "Started creating L0 packet writer" );

#ifdef USE_INTERNAL_ALLOCATOR
	sL0PacketWriter *PW = &StaticL0Writer;
#else
	sL0PacketWriter *PW = (sL0PacketWriter*)EmbededMalloc( sizeof( sL0PacketWriter ) );
#endif
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

//	PW->FileName = EmbededStrDup( FName );

	PW->Lane = pLaneNr;
	PW->HostID = pHostID;
	PW->DeviceID = pDeviceID;

	Dprintf( DLVerbose, "\t Finished creating L0 packet writer. All OK" );

	return PW;
}

void DestroyL0PacketWriter( sL0PacketWriter **PW )
{
	Dprintf( DLVerbose, "Started destroying L0 packet writer" );
//	if( (*PW)->FileName )
//	{
//		EmbededFree( (*PW)->FileName );
//		(*PW)->FileName = NULL;
//	}
	if( (*PW)->File )
	{
		fclose( (*PW)->File );
		(*PW)->File = NULL;
	}
#ifndef USE_INTERNAL_ALLOCATOR
	EmbededFree( (*PW) );
#endif
	*PW = NULL;
	Dprintf( DLVerbose, "\t Finished destroying L0 packet writer" );
}

int L1L0ProcessLine( sL0PacketWriter *PW, const BYTE *Data, int DataLen )
{
	Dprintf( DLVerbose, "Started PW process 1 line. Write %d bytes", DataLen );
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
	size_t written = fwrite( Data, 1, DataLen, PW->File );

	char FormattedPacket[ MAX_PACKET_SIZE ];
	strcpy_s( FormattedPacket, MAX_PACKET_SIZE, "" );
	for( int i=0;i<DataLen && i<MAX_PACKET_SIZE;i++)
		sprintf_s( FormattedPacket, MAX_PACKET_SIZE, "%s%02X", FormattedPacket, Data[i] );

	Dprintf( DLVerbose, "\t Finished PW process 1 line : %s", FormattedPacket );
	return 0;
}