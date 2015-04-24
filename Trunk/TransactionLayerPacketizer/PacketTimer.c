#include "StdAfx.h"

void PacketSendStartTimer( struct TransactionLayerPacket *Packet )
{
	Packet->TimeStampSent = 1;
}

void PacketRecvStopTimer( struct TransactionLayerPacket *Packet )
{
	Packet->TimeStampRecv = 2;
}

int GetTransactionDuration( int TransactionId )
{
	int i;
	int TotalTransactionDuration = 0;
	for( i=0;i<MAX_PACKETS_TO_QUEUE;i++)
		if( PacketStoreCache[i].TransactionId == TransactionId && PacketStoreCache[i].PacketSizeResponse > 0 )
		{
			int TimeDelta = PacketStoreCache[i].TimeStampRecv - PacketStoreCache[i].TimeStampSent;
			TotalTransactionDuration += TimeDelta;
		}
	return TotalTransactionDuration;
}