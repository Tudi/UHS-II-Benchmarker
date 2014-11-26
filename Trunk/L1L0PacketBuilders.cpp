#include "StdAfx.h"

void	L1BuildPacketSTBL( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)malloc( 1 );
	(*Data)[0] = 0;
	*DataLen = 1;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPacketSYN( BYTE **Data, int *DataLen, char *Line )
{
	*Data = (BYTE *)malloc( 2 );
	(*Data)[0] = LSS_COM;
	(*Data)[1] = LSS_SYN0;
	*DataLen = 2;
	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPacketReadInitialize( BYTE **Data, int *DataLen, char *Line )
{
	*DataLen = sizeof( sFullLinkLayerPacketDCMD );
	sFullLinkLayerPacketDCMD *p = (sFullLinkLayerPacketDCMD *)malloc( *DataLen );
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
	p->Packet.ReadWrite = 0;
	p->Packet.Reserved1 = 0;
	p->Packet.Addr = GetLineParamXInteger( Line, 0 );
	p->Packet.DataLen = GetLineParamXInteger( Line, 1 );

	p->CRC = crc16_ccitt( *Data + 2, *DataLen - 2 );

	p->EOPLSS[0] = LSS_COM;
	p->EOPLSS[1] = LSS_EOP;

	*Data = DuplicatePacket( Data, DataLen, Line );
}

void	L1BuildPacketSTBH( BYTE **Data, int *DataLen, char *Line )
{
}


void	L1BuildPacketBSYN( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketDIR( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketLIDL( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketDIDL( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketSDB( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketSOP( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketEOP( BYTE **Data, int *DataLen, char *Line )
{
}

void	L1BuildPacketEDB( BYTE **Data, int *DataLen, char *Line )
{
}
