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

	errno_t er = fopen_s( &PR->File, FName, "rb" );
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

int IsValidPacket( BYTE *ByteStream, int *AvailableBytes )
{
	for( int i=0;i<L1SymbolListSize;i++)
	{
		BYTE *PacketEndAt = ByteStream;
		char *StringPacket = L1SymbolList[i]->PacketParser( &PacketEndAt, AvailableBytes );
		if( StringPacket != NULL )
		{
			free( StringPacket );
			return i;
		}
	}
	return -1;
}

int ReadNextPacket( sL0PacketReader *PR )
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

	BYTE	PacketBuffer[MAX_PACKET_SIZE];
	int		ReadIndex = 0;

	int ReadCount = fread( PacketBuffer, 1, MAX_PACKET_SIZE, PR->File );

	//read unknown data until next resync marker. Best case this is "0" length
	int AvailableBytes = ReadCount;
	int SelectedHandler;
	SelectedHandler = IsValidPacket( &PacketBuffer[ReadIndex], &AvailableBytes );
	//how many non valid bytes can we read one by one ?
	while( SelectedHandler == -1 && ReadIndex < ReadCount )
	{
		ReadIndex++;
		AvailableBytes = ReadCount - ReadIndex;
		SelectedHandler = IsValidPacket( &PacketBuffer[ReadIndex], &AvailableBytes );
	}

	if( ReadIndex > 0 )
	{
		if( ReadCount > 1 )
			AvailableBytes++;
		SelectedHandler = UNKNOWN_BYTES_PACKET_HANDLER_INDEX;
	}

	//rewind file of unused bytes
	fseek( PR->File, -AvailableBytes, SEEK_CUR );
	PR->PacketCounter++;
	PR->StreamBytesRead += ReadCount - AvailableBytes;

	//backup previous packet in case we need it later to see if a packet is getting repeated
	memset( PR->PacketBufferPrevious, 0, MAX_PACKET_SIZE );	
	PR->PacketSizePrev = PR->PacketSize;
	memcpy( PR->PacketBufferPrevious, PR->PacketBuffer, PR->PacketSizePrev );

	//copy from temp to storage our packet or block of data that we can handle
	memset( PR->PacketBuffer, 0, MAX_PACKET_SIZE );	//just in case shit happens, we can be sure to not read from previous line data
	PR->PacketSize = ReadCount - AvailableBytes;
	memcpy( PR->PacketBuffer, PacketBuffer, PR->PacketSize );

	Dprintf( DLVerbose, "\t Finished PR Read 1 line" );

	if( feof( PR->File ) )
		return -1;

	return SelectedHandler;
}

int L0PacketReaderProcessFile( sL0PacketReader *PR, sL1PacketWriter *PW )
{
	Dprintf( DLVerbose, "Started PR process whole file" );
	int RetHandlerIndex = ReadNextPacket( PR );
	while( RetHandlerIndex > -1 )
	{
		Dprintf( DLVerbose, "\t PR read packet at stamp %d, at buffer index 0x%04X, packet Nr %04d", PR->StreamBytesRead, PR->StreamBytesRead, PR->PacketCounter );

		//try to build a packet that packet writer can write to file
		BYTE	*NextPacketAt = PR->PacketBuffer;
		int		RemainingBytes = PR->PacketSize;
		char	*String;

		if( RetHandlerIndex != UNKNOWN_BYTES_PACKET_HANDLER_INDEX )
			String = L1SymbolList[ RetHandlerIndex ]->PacketParser( &NextPacketAt, &RemainingBytes );
		else
			String = L0ParsePckt_Unk( &NextPacketAt, &RemainingBytes );

		if( String != NULL )
			L0L1ProcessPacket( PW, String );

		free( String );

		RetHandlerIndex = ReadNextPacket( PR );
	}
	Dprintf( DLVerbose, "\t Finished PR process whole file" );
	return 0;
}
