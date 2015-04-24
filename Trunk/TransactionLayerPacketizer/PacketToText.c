#include "StdAfx.h"

void FormatToTextCCMD( TLPU_CCMD *P_CCMD )
{
	xil_printf( "CCMD->Header->DestinationID : %d \n", P_CCMD->Fields.Header.Fields.DestinationID );
	xil_printf( "CCMD->Header->PacketType : %d \n", P_CCMD->Fields.Header.Fields.PacketType );
	xil_printf( "CCMD->Header->NativePacket : %d \n", P_CCMD->Fields.Header.Fields.NativePacket );
	xil_printf( "CCMD->Header->TransactionID : %d \n", P_CCMD->Fields.Header.Fields.TransactionID );
	xil_printf( "CCMD->Header->Reserved : %d \n", P_CCMD->Fields.Header.Fields.Reserved );
	xil_printf( "CCMD->Header->SourceID : %d \n", P_CCMD->Fields.Header.Fields.SourceID );
	xil_printf( "CCMD->Header->DestinationID : %d \n", P_CCMD->Fields.Header.Fields.DestinationID );

	xil_printf( "CCMD->Argument->IOADDR1 : %d \n", P_CCMD->Fields.Argument.Fields.IOADDR1 );
	xil_printf( "CCMD->Argument->PLEN : %d \n", P_CCMD->Fields.Argument.Fields.PLEN );
	xil_printf( "CCMD->Argument->Reserved : %d \n", P_CCMD->Fields.Argument.Fields.Reserved );
	xil_printf( "CCMD->Argument->ReadWrite : %d \n", P_CCMD->Fields.Argument.Fields.ReadWrite );
	xil_printf( "CCMD->Argument->IOADDR0 : %d \n", P_CCMD->Fields.Argument.Fields.IOADDR0 );
}

void FormatToTextCCMDResp( TLPU_RES *P_RES )
{
	//header is compatible, Only payload is different
	FormatToTextCCMD( (TLPU_CCMD *)P_RES );
}

void FormatToTextTransactionRoundTripTimes( int TransactionId )
{
	int i;
	for( i=0;i<MAX_PACKETS_TO_QUEUE;i++)
		if( PacketStoreCache[i].TransactionId == TransactionId && PacketStoreCache[i].PacketSizeResponse > 0 )
		{
			TLPU_Header *P_HSend;
			TLPU_Header *P_HRecv;
			int TimeDelta = PacketStoreCache[i].TimeStampRecv - PacketStoreCache[i].TimeStampSent;
			P_HSend = (TLPU_Header*)&PacketStoreCache[i].Packet[0];
			P_HRecv = (TLPU_Header*)&PacketStoreCache[i].PacketResponse[0];
			xil_printf( "Packet Type sent : %d. Packet Type Recv %d. Roundtrip duration %d\n", P_HSend->Fields.PacketType, P_HRecv->Fields.PacketType, TimeDelta );
		}
}