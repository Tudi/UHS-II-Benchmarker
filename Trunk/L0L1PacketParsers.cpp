#include "StdAfx.h"

char *DummyPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}

char *UnkPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	char *Ret = (char *)malloc( 100 + *AvailableBytes * 2 + 3 );
	sprintf_s( Ret, 100, "0 UnkData %04d : ", *AvailableBytes );
	for( int i=0;i<*AvailableBytes;i++)
		sprintf_s( Ret, 100 + *AvailableBytes * 2 + 3, "%s%02X", Ret, (*ReadStream)[i] );

	*ReadStream = &(*ReadStream)[ *AvailableBytes - 1 ];
	*AvailableBytes = 0;
	return Ret;
}

char *PacketParserHandlerSTBL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != STBL )
		return NULL;

	char *Ret = (char *)malloc( 103 );
	sprintf_s( &Ret[0], 103, "0 STBL 1 : 0x%02X", RS[0] );

	*ReadStream = &RS[ 1 ];
	*AvailableBytes = *AvailableBytes - 1;
	return Ret;
}

char *PacketParserHandlerSYN( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_SYN0 && RS[1] != LSS_SYN1 ) )
		return NULL;

	char *Ret = (char *)malloc( 104 );
	sprintf_s( &Ret[0], 104, "0 SYN 1 : " );
	for( int i=0;i<2;i++)
		sprintf_s( Ret, 104, "%s%02X", Ret, RS[ i ] );

	*ReadStream = &RS[ 2 ];
	*AvailableBytes = *AvailableBytes - 2;
	return Ret;
}

char *PacketParserHandlerLIDL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_LIDL0 && RS[1] != LSS_LIDL1 ) )
		return NULL;

	char *Ret = (char *)malloc( 104 );
	sprintf_s( &Ret[0], 104, "0 LIDL 1 : " );
	for( int i=0;i<2;i++)
		sprintf_s( Ret, 104, "%s%02X", Ret, RS[ i ] );

	*ReadStream = &RS[ 2 ];
	*AvailableBytes = *AvailableBytes - 2;
	return Ret;
}

char *PacketParserHandlerGenericPacket( BYTE **ReadStream, int *AvailableBytes )
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
		Ret = PacketParserHandlerDCMD( &SubReadStream, &subAvailable );
	}

	if( Ret != NULL )
	{
		//check the CRC of the packet
		int PacketCRCFromUs = crc16_ccitt( *ReadStream + 2, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) );
		int PacketCRCFromPacket = *(unsigned short *)SubReadStream;
		if( PacketCRCFromUs != PacketCRCFromPacket )
		{
			free( Ret );

			Ret = (char *)malloc( 1000 );
			sprintf_s( &Ret[0], 1000, "0 CRC (us)%d - (packet)%d Failed on packet : ", PacketCRCFromUs, PacketCRCFromPacket );
			for( int i=0;i<(int)( SubReadStream - *ReadStream);i++)
				sprintf_s( Ret, 1000, "%s%02X", Ret, RS[ i ] );

		}
		*ReadStream = SubReadStream;
		*AvailableBytes = subAvailable;
	}

	return Ret;
}

char *PacketParserHandlerDCMD( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	sLinkLayerPacketHeader *PH = (sLinkLayerPacketHeader *)&RS[2];

	if( PH->PacketType != LLPT_DCMD )
		return NULL;

	sFullLinkLayerPacketDCMD *PDCMD = (sFullLinkLayerPacketDCMD *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	char *Ret = (char *)malloc( 1000 );
	sprintf_s( &Ret[0], 1000, "0 DCMD 1 : " );
	for( int i=0;i<2;i++)
		sprintf_s( Ret, 1000, "%s%02X", Ret, RS[ i ] );

	int DCMDPacketSize = sizeof( sFullLinkLayerPacketDCMD );
	*AvailableBytes = *AvailableBytes - DCMDPacketSize;

	*ReadStream = *ReadStream + DCMDPacketSize;

	return Ret;
}