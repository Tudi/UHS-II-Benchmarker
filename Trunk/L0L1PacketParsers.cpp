#include "StdAfx.h"

char *L0ParsePckt_Unk( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	char *Ret = (char *)malloc( MAX_PACKET_SIZE + *AvailableBytes * 2 + 3 );
	sprintf_s( Ret, MAX_PACKET_SIZE, "0 UnkData %04d : ", *AvailableBytes );
	for( int i=0;i<*AvailableBytes;i++)
		sprintf_s( Ret, MAX_PACKET_SIZE + *AvailableBytes * 2 + 3, "%s%02X", Ret, (*ReadStream)[i] );

	*ReadStream = &(*ReadStream)[ *AvailableBytes - 1 ];
	*AvailableBytes = 0;
	return Ret;
}

char *L0ParsePckt_STBL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != STBL )
		return NULL;

	int		PacketSize = 1;
	int		CanSkipLocations[] = { -1 };
	int		CanSkipLocationValue[] = { -1 };
	// try to detect and handle packet duplication
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "STB.L" );
}

char *L0ParsePckt_SYN( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_SYN0 && RS[1] != LSS_SYN1 ) )
		return NULL;

	int		PacketSize = 2;
	int		CanSkipLocations[] = { -1 };
	int		CanSkipLocationValue[] = { -1 };
	// try to detect and handle packet duplication
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "SYN" );
}

char *L0ParsePckt_LIDL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_LIDL0 && RS[1] != LSS_LIDL1 ) )
		return NULL;

	int		PacketSize = 2;
	int		CanSkipLocations[] = { -1 };
	int		CanSkipLocationValue[] = { -1 };
	// try to detect and handle packet duplication
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "LIDL" );
}

char *L0ParsePckt_DCMD( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketDCMD );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sLinkLayerPacketHeader *PH = (sLinkLayerPacketHeader *)&RS[2];

	if( PH->PacketType != LLPT_DCMD )
		return NULL;

	sFullLinkLayerPacketDCMD *PDCMD = (sFullLinkLayerPacketDCMD *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	//check the CRC of the packet
	int PacketCRCFromUs = crc16_ccitt( RS + 2, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) );
	int PacketCRCFromPacket = PDCMD->CRC;

	int		CanSkipLocations[] = { -1 };
	int		CanSkipLocationValue[] = { -1 };
	// try to detect and handle packet duplication
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "DCMD" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	if( PDCMD->Packet.ReadWrite == 0 )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Read", Ret );
	else
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Write", Ret );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Addr( %d ) Count( %d ) Mode( %d )", Ret, PDCMD->Packet.Addr, PDCMD->Packet.DataLen, PDCMD->Packet.TMode );

	return Ret;
}

/*
char *L0ParsePckt_GenericPacket( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || RS[1] != LSS_SOP )
		return NULL;

	sLinkLayerPacketHeader *PH = (sLinkLayerPacketHeader *)&RS[2];

	int		subAvailable = *AvailableBytes;
	BYTE	*SubReadStream = *ReadStream;
	char	*Ret = NULL;

	if( PH->PacketType == LLPT_DCMD )
	{
		Ret = L0ParsePckt_DCMD( &SubReadStream, &subAvailable );
	}

	if( Ret != NULL )
	{
		*ReadStream = SubReadStream;
		*AvailableBytes = subAvailable;
	}

	return Ret;
}*/