#include "StdAfx.h"

void	L1BuildPckt_STBL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 1 );
	(*Data)[0] = 0;
	*DataLen = 1;
	*Data = DuplicatePacket( Data, DataLen, Line );
	Dprintf( DLVerbose, "\t PB built STBL packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_SYN( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_SYN0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line, 1 );
	Dprintf( DLVerbose, "\t PB built SYN packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_DCMD( BYTE **Data, int *DataLen, char *Line )
{
	*DataLen = sizeof( sFullLinkLayerPacketDCMD );
	sFullLinkLayerPacketDCMD *p = (sFullLinkLayerPacketDCMD *)EmbededMalloc( *DataLen );
	*Data = (BYTE*)p;

	p->SOPLSS[0] = LSS_COM;
	p->SOPLSS[1] = LSS_SOP;

	p->Header.DestinationID = 0;
	p->Header.PacketType = LLPT_DCMD;
	p->Header.NativePacket = 1;	
	p->Header.TransactionID = 0;
	p->Header.Reserved = 0;
	p->Header.SourceID = 0;

	p->Packet.Reserved0 = 0;
	p->Packet.TModeDuplexMode = GetLineParamXInteger( Line, 1 );
	p->Packet.TModeLengthMode = GetLineParamXInteger( Line, 2 );
	p->Packet.TModeTLUnitMode = GetLineParamXInteger( Line, 3 );
	p->Packet.TModeDataAccessMode = GetLineParamXInteger( Line, 4 );
	p->Packet.ReadWrite = GetLineParamXInteger( Line, 0 );	//needs to be 0 for read
	p->Packet.Reserved1 = 0;
	p->Packet.Addr = GetLineParamXInteger( Line, 5 );
	p->Packet.DataLen = GetLineParamXInteger( Line, 6 );

	p->CRC = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) ) );

	p->EOPLSS[0] = LSS_COM;
	p->EOPLSS[1] = LSS_EOP;

	ScramblePacket( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) + sizeof( p->CRC ) );

	*Data = DuplicatePacket( Data, DataLen, Line );
	Dprintf( DLVerbose, "\t PB built DCMDR packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_LIDL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_LIDL0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line, 1 );
	Dprintf( DLVerbose, "\t PB built LIDL packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_DIDL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_DIDL0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line, 1 );
	Dprintf( DLVerbose, "\t PB built DIDL packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_RES( BYTE **Data, int *DataLen, char *Line )
{
	*DataLen = sizeof( sFullLinkLayerPacketRES );
	sFullLinkLayerPacketRES *p = (sFullLinkLayerPacketRES *)EmbededMalloc( *DataLen );
	*Data = (BYTE*)p;

	p->SOPLSS[0] = LSS_COM;
	p->SOPLSS[1] = LSS_SOP;

	p->Header.DestinationID = 0;
	p->Header.PacketType = LLPT_RES;
	p->Header.NativePacket = 1;	
	p->Header.TransactionID = 0;
	p->Header.Reserved = 0;
	p->Header.SourceID = 0;

	int CommandToAccept = GetLineParamXInteger( Line, 0 );
	p->Packet.NAck = 0;		//0 means we accept the command sent to us
	p->Packet.CMD_ECHO_BACK0 = CommandToAccept & 0x7F;		//lower 7 bits
	p->Packet.CMD_ECHO_BACK1 = CommandToAccept >> 7;		//higher 8 bits

	p->CRC = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketRES ) ) );

	p->EOPLSS[0] = LSS_COM;
	p->EOPLSS[1] = LSS_EOP;

	ScramblePacket( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketRES ) + sizeof( p->CRC ) );

	*Data = DuplicatePacket( Data, DataLen, Line );
	Dprintf( DLVerbose, "\t PB built RES packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_DATA( BYTE **Data, int *DataLen, char *Line )
{
	BYTE Payload[ MAX_PACKET_SIZE ];
	int PayloadLength = GetLineParamXHexSTR( Line, 0, Payload, MAX_PACKET_SIZE );

	*DataLen = sizeof( sFullLinkLayerPacketDATA0 ) + PayloadLength + sizeof( sFullLinkLayerPacketDATA1 );
	sFullLinkLayerPacketDATA0 *p0 = (sFullLinkLayerPacketDATA0 *)EmbededMalloc( *DataLen );
	*Data = (BYTE*)p0;
	sFullLinkLayerPacketDATA1 *p1 = (sFullLinkLayerPacketDATA1 *)( *Data + sizeof( sFullLinkLayerPacketDATA0 ) + PayloadLength );

	p0->SOPLSS[0] = LSS_COM;
	p0->SOPLSS[1] = LSS_SOP;

	p0->Header.DestinationID = 0;
	p0->Header.PacketType = LLPT_DATA;
	p0->Header.NativePacket = 1;	
	p0->Header.TransactionID = 0;
	p0->Header.Reserved = 0;
	p0->Header.SourceID = 0;

	memcpy( *Data + sizeof( sFullLinkLayerPacketDATA0 ), Payload, PayloadLength );

	p1->CRC = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&p0->Header, sizeof( sLinkLayerPacketHeader ) + PayloadLength ) );

	p1->EOPLSS[0] = LSS_COM;
	p1->EOPLSS[1] = LSS_EOP;

	ScramblePacket( (BYTE*)&p0->Header, sizeof( sLinkLayerPacketHeader ) + PayloadLength + sizeof( p1->CRC ) );

	*Data = DuplicatePacket( Data, DataLen, Line );
	Dprintf( DLVerbose, "\t PB built DATA packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_CCMDDI( BYTE **Data, int *DataLen, char *Line )
{
	*DataLen = sizeof( sFullLinkLayerPacketCCMDDI );
	sFullLinkLayerPacketCCMDDI *p = (sFullLinkLayerPacketCCMDDI *)EmbededMalloc( *DataLen );
	*Data = (BYTE*)p;

	p->SOPLSS[0] = LSS_COM;
	p->SOPLSS[1] = LSS_SOP;

	p->Header.DestinationID = 0;
	p->Header.PacketType = LLPT_CCMD;
	p->Header.NativePacket = 1;	
	p->Header.TransactionID = 0;
	p->Header.Reserved = 0;
	p->Header.SourceID = 0;

	p->Packet.IOADDR0 = 0;
	p->Packet.PLEN = 1;
	p->Packet.Reserved = 0;
	p->Packet.ReadWrite = 1;
	p->Packet.IOADDR1 = 0;

	p->PacketDeviceInit.GD = 0;	//group descriptor
	p->PacketDeviceInit.GAP = 3; //group allocated power : 1080mW -> see power levels on page 153
	p->PacketDeviceInit.DAP = 3; //device allocated power : 1080mW -> see power levels on page 153
	p->PacketDeviceInit.CF = 1; //completion flag, mark the device as initialized

	p->CRC = CRC_LSB_SWAP( crc16_ccitt( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDI ) ) );

	p->EOPLSS[0] = LSS_COM;
	p->EOPLSS[1] = LSS_EOP;

	ScramblePacket( (BYTE*)&p->Header, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketCCMD ) + sizeof( sLinkLayerPacketCCMDDI ) + sizeof( p->CRC ) );

	*Data = DuplicatePacket( Data, DataLen, Line );
	Dprintf( DLVerbose, "\t PB built CCMDDI packet. Total size : %d bytes", *DataLen );
}

void	L1BuildPckt_CCMDR( BYTE **Data, int *DataLen, char *Line )
{

}

void	L1BuildPckt_CCMDW( BYTE **Data, int *DataLen, char *Line )
{

}

void	L1BuildPckt_FCRDY( BYTE **Data, int *DataLen, char *Line )
{

}
