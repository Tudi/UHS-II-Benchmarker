#include "StdAfx.h"

char *L0ParsePckt_Unk( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	if( *AvailableBytes <= 0 )
		return NULL;

	int	PacketSize = *AvailableBytes;
	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int	PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	Dprintf( DLVerbose, "\t PP read UNK packet. Total size : %d bytes", ProcessedByteCount );
	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "UnkData" );
}

char *L0ParsePckt_STBL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != STBL )
		return NULL;

	int	PacketSize = 1;
	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int	PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	Dprintf( DLVerbose, "\t PP read STB.L packet. Total size : %d bytes", ProcessedByteCount );
	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "STB.L" );
}

char *L0ParsePckt_SYN( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_SYN0 && RS[1] != LSS_SYN1 ) )
		return NULL;

	int	PacketSize = 2;
	int	CanSkipLocations[] = { 1, 1, -1 };
	int	CanSkipLocationValue[] = { LSS_SYN0, LSS_SYN1, -1 };
	int	PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	Dprintf( DLVerbose, "\t PP read SYN packet. Total size : %d bytes", ProcessedByteCount );
	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "SYN" );
}

char *L0ParsePckt_LIDL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_LIDL0 && RS[1] != LSS_LIDL1 ) )
		return NULL;

	int	PacketSize = 2;
	int	CanSkipLocations[] = { 1, 1, -1 };
	int	CanSkipLocationValue[] = { LSS_LIDL0, LSS_LIDL1, -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	Dprintf( DLVerbose, "\t PP read LIDL packet. Total size : %d bytes", ProcessedByteCount );
	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "LIDL" );
}

char *L0ParsePckt_DIDL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_DIDL0 && RS[1] != LSS_DIDL1 ) )
		return NULL;

	int	PacketSize = 2;
	int	CanSkipLocations[] = { 1, 1, -1 };
	int	CanSkipLocationValue[] = { LSS_DIDL0, LSS_DIDL1, -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	Dprintf( DLVerbose, "\t PP read DIDL packet. Total size : %d bytes", ProcessedByteCount );
	return GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "DIDL" );
}

char *L0ParsePckt_DCMD( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketDCMD );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketDCMD *PDCMD = (sFullLinkLayerPacketDCMD *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	sFullLinkLayerPacketDCMD TempPacket;

	memcpy( &TempPacket, RS, sizeof( TempPacket ) );
	ScramblePacket( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) + sizeof( TempPacket.CRC ) );

	if( TempPacket.Header.PacketType != LLPT_DCMD )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) ) );
	int PacketCRCFromPacket = TempPacket.CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "DCMD" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	if( PDCMD->Packet.ReadWrite == 0 )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Read", Ret );
	else
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Write", Ret );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Addr( %d ) Count( %d ) Duplex Mode( %d ) Length Mode( %d ) Unit Mode( %d ) Data Access Mode( %d )", Ret, TempPacket.Packet.Addr, TempPacket.Packet.DataLen, TempPacket.Packet.TModeDuplexMode, TempPacket.Packet.TModeLengthMode, TempPacket.Packet.TModeTLUnitMode, TempPacket.Packet.TModeDataAccessMode );

	Dprintf( DLVerbose, "\t PP read DCMD packet. Total size : %d bytes", ProcessedByteCount );
	return Ret;
}

char *L0ParsePckt_RES( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketDCMD );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketRES *PDCMD = (sFullLinkLayerPacketRES *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	int			PayloadSize = 0;
	BYTE		TempPacketBuffer[ MAX_PACKET_SIZE ];
	sFullLinkLayerPacketRES *TempPacket = (sFullLinkLayerPacketRES *)TempPacketBuffer;
RESTART_PARSING_WITH_DIFFERENT_PAYLOAD_SIZE:
	PacketSize = sizeof( sFullLinkLayerPacketRES ) + PayloadSize;
	memcpy( TempPacket, RS, PacketSize );
	ScramblePacket( (BYTE*)&TempPacket->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketRES ) + PayloadSize + sizeof( TempPacket->CRC ) );

	if( TempPacket->Header.PacketType != LLPT_RES )
		return NULL;

	//let's make some guesses what kind of RES packet is this
	sLinkLayerPacketCCMD *TempCCMDRES = (sLinkLayerPacketCCMD *)&TempPacket->Packet;
	if( TempCCMDRES->PLEN > 0 && PayloadSize == 0 )
	{
		PayloadSize = TempCCMDRES->PLEN * 4;
		goto RESTART_PARSING_WITH_DIFFERENT_PAYLOAD_SIZE;
	}

	if( RS[ PacketSize - 2 ] != LSS_COM || RS[ PacketSize - 1 ] != LSS_EOP )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&TempPacket->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketRES ) + PayloadSize ) );
	int PacketCRCFromPacket = TempPacket->CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "RES" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	if( TempPacket->Packet.NAck == 0 )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Accepted", Ret );
	else
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Rejected", Ret );

	//for a register read this will contain PLEN / IOADDR and probably 4 bytes of payload
	int FullCommand = TempPacket->Packet.CMD_ECHO_BACK1 | ( ( TempPacket->Packet.CMD_ECHO_BACK0 & 0x7F ) << 8 );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Command( %d )", Ret, FullCommand );

	if( TempCCMDRES->PLEN > 0 || TempCCMDRES->IOADDR1 > 0 )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s Possible CCMD RES with len(%d) addr( %d )", Ret, TempCCMDRES->PLEN, TempCCMDRES->IOADDR1 );

	Dprintf( DLVerbose, "\t PP read RES packet. Total size : %d bytes", ProcessedByteCount );
	return Ret;
}

char *L0ParsePckt_DATA( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PayloadLength = READ_WRITE_BLOCK_LENGTH;
	int PacketSize = sizeof( sFullLinkLayerPacketDATA0 ) + PayloadLength + sizeof( sFullLinkLayerPacketDATA1 );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketDATA0 *PDCMD0 = (sFullLinkLayerPacketDATA0 *)*ReadStream;
	sFullLinkLayerPacketDATA1 *PDCMD1 = (sFullLinkLayerPacketDATA1 *)(*ReadStream + sizeof( sFullLinkLayerPacketDATA0 ) + PayloadLength);

	if( PDCMD0->SOPLSS[0] != LSS_COM || PDCMD0->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD1->EOPLSS[0] != LSS_COM || PDCMD1->EOPLSS[1] != LSS_EOP )
		return NULL;

	BYTE	TempPacket[ MAX_PACKET_SIZE ];

	memcpy( TempPacket, RS, PacketSize );
	ScramblePacket( TempPacket + 2, PacketSize - 4 );
	PDCMD0 = (sFullLinkLayerPacketDATA0 *)TempPacket;
	PDCMD1 = (sFullLinkLayerPacketDATA1 *)(TempPacket + sizeof( sFullLinkLayerPacketDATA0 ) + PayloadLength);

	if( PDCMD0->Header.PacketType != LLPT_DATA )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&PDCMD0->Header, sizeof( sLinkLayerPacketHeader ) + PayloadLength ) );
	int PacketCRCFromPacket = PDCMD1->CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "DATA" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Unscrambled data :", Ret );
	for( int i=0;i<PayloadLength;i++)
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s %02X", Ret, TempPacket[ + sizeof( sFullLinkLayerPacketDATA0 ) + i ] );

	Dprintf( DLVerbose, "\t PP read DATA packet. Total size : %d bytes", ProcessedByteCount );
	return Ret;
}

//this is a big hack to simulate device initialization multiple states parsing the same packet differently. This hack presumes we can not reinitialize device using software( or you need to remove this hack )
static int DeviceIsInitialized = 0;
static int DeviceIsEnumerated = 0;

char *L0ParsePckt_CCDMDI_H( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketCCMDDI );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketCCMDDI *PDCMD = (sFullLinkLayerPacketCCMDDI *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	sFullLinkLayerPacketCCMDDI TempPacket;

	memcpy( &TempPacket, RS, sizeof( TempPacket ) );
	ScramblePacket( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDI ) + sizeof( TempPacket.CRC ) );

	if( TempPacket.Header.PacketType != LLPT_CCMD )
		return NULL;

	if( DeviceIsEnumerated == 0 || DeviceIsInitialized > 1 )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDI ) ) );
	int PacketCRCFromPacket = TempPacket.CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "CCDMDI" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Device power( %d )", Ret, TempPacket.PacketDeviceInit.DAP );
	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Completion flag( %d )", Ret, TempPacket.PacketDeviceInit.CF );

	Dprintf( DLVerbose, "\t PP read CCMDDI packet. Total size : %d bytes", ProcessedByteCount );

	if( TempPacket.PacketDeviceInit.CF == 1 )
		DeviceIsInitialized = DeviceIsInitialized + 1;

	return Ret;
}

char *L0ParsePckt_CCDMDE_H( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketCCMDDE );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketCCMDDE *PDCMD = (sFullLinkLayerPacketCCMDDE *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	sFullLinkLayerPacketCCMDDE TempPacket;

	memcpy( &TempPacket, RS, sizeof( TempPacket ) );
	ScramblePacket( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDE ) + sizeof( TempPacket.CRC ) );

	if( TempPacket.Header.PacketType != LLPT_CCMD )
		return NULL;

	if( DeviceIsEnumerated > 1 )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDE ) ) );
	int PacketCRCFromPacket = TempPacket.CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "CCDMDE" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s FirstNode( %d )", Ret, TempPacket.PacketDeviceEnum.FirstNodeID );
	sprintf_s( Ret, MAX_PACKET_SIZE, "%s LastNode( %d )", Ret, TempPacket.PacketDeviceEnum.LastNodeID );

	Dprintf( DLVerbose, "\t PP read CCMDDE packet. Total size : %d bytes", ProcessedByteCount );

	DeviceIsEnumerated = DeviceIsEnumerated + 1;

	return Ret;
}

char *L0ParsePckt_GETSETREG( BYTE **ReadStream, int *AvailableBytes )
{
	BYTE *RS = *ReadStream;
	int PacketSize = sizeof( sFullLinkLayerPacketRegisterInquery );
	if( *AvailableBytes < PacketSize )
		return NULL;

	sFullLinkLayerPacketRegisterInquery *PDCMD = (sFullLinkLayerPacketRegisterInquery *)*ReadStream;

	if( PDCMD->SOPLSS[0] != LSS_COM || PDCMD->SOPLSS[1] != LSS_SOP )
		return NULL;

	if( PDCMD->EOPLSS[0] != LSS_COM || PDCMD->EOPLSS[1] != LSS_EOP )
		return NULL;

	sFullLinkLayerPacketRegisterInquery TempPacket;

	memcpy( &TempPacket, RS, sizeof( TempPacket ) );
	ScramblePacket( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( TempPacket.data ) + sizeof( TempPacket.CRC ) );

	if( TempPacket.Header.PacketType != LLPT_CCMD )
		return NULL;

	if( DeviceIsEnumerated <= 1 || DeviceIsInitialized <= 1 )
		return NULL;

	int	CanSkipLocations[] = { -1 };
	int	CanSkipLocationValue[] = { -1 };
	int PacketCount = CountPacketDuplicat( ReadStream, AvailableBytes, PacketSize, CanSkipLocations, CanSkipLocationValue );
	int	ProcessedByteCount = PacketCount * PacketSize;

	//check the CRC of the packet
	int PacketCRCFromUs = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&TempPacket.Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( TempPacket.data ) ) );
	int PacketCRCFromPacket = TempPacket.CRC;

	char *Ret = GenericFormatPacketAsHex( RS, ProcessedByteCount, PacketSize, "REGISTER" );

	if( PacketCRCFromUs != PacketCRCFromPacket )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s CRC_FAILED_(us)%d-(packet)%d", Ret, PacketCRCFromUs, PacketCRCFromPacket );

	if( TempPacket.HeaderCCMD.ReadWrite == 1 )
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s OP(W)", Ret );
	else
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s OP(R)", Ret );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Addr(%d), Size(%d)", Ret, TempPacket.HeaderCCMD.IOADDR1, TempPacket.HeaderCCMD.PLEN );

	sprintf_s( Ret, MAX_PACKET_SIZE, "%s Unscrambled data :", Ret );
	for( int i=0;i<TempPacket.HeaderCCMD.PLEN * 4;i++)
		sprintf_s( Ret, MAX_PACKET_SIZE, "%s %02X", Ret, TempPacket.data[ i ] );

	PrintRegisterContent( TempPacket.HeaderCCMD.IOADDR1, TempPacket.HeaderCCMD.PLEN, TempPacket.data );

	Dprintf( DLVerbose, "\t PP read REGISTER packet. Total size : %d bytes", ProcessedByteCount );

	return Ret;
}
/*
char *L0ParsePckt_FCRDY( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}

char *L0ParsePckt_CCMD( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}
*/