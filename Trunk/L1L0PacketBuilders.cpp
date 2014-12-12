#include "StdAfx.h"

void	L1BuildPckt_STBL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 1 );
	(*Data)[0] = 0;
	*DataLen = 1;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPckt_SYN( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_SYN0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPckt_DCMDR( BYTE **Data, int *DataLen, char *Line )
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
	p->Packet.TMode = LSS_COM;
	p->Packet.ReadWrite = 0;	//needs to be 0 for read
	p->Packet.Reserved1 = 0;
	p->Packet.Addr = GetLineParamXInteger( Line, 0 );
	p->Packet.DataLen = GetLineParamXInteger( Line, 1 );

	p->CRC = CRC_LSB_SWAP( crc16_ccitt( *Data + 2, sizeof( sLinkLayerPacketHeader ) + sizeof( sLinkLayerPacketDCMD ) ) );

	p->EOPLSS[0] = LSS_COM;
	p->EOPLSS[1] = LSS_EOP;

	ScramblePacket( *Data + 2 , *DataLen - 4 );

	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPckt_LIDL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_LIDL0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPckt_DIDL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)EmbededMalloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = GetRandomizedOpcode( LSS_DIDL0 );
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPckt_FCRDY( BYTE **Data, int *DataLen, char *Line )
{

}
