#include "StdAfx.h"

#define MAX_PACKETS_TO_QUEUE	50
struct TransactionLayerPacket	PacketStoreCache[MAX_PACKETS_TO_QUEUE];

void InitPacketQueueForNewTestCase()
{
	int i;
	for( i = 0; i < MAX_PACKETS_TO_QUEUE; i++ )
	{
		EmbededMemSet( (char *)&PacketStoreCache[i], 0 , sizeof( MAX_PACKETS_TO_QUEUE ) );
		PacketStoreCache[i].PacketState = PS_PACKET_IS_FREE;
		PacketStoreCache[i].SelfIndex = i;
		PacketStoreCache[i].SendCount = 1;
	}
}

struct TransactionLayerPacket *GetPacketQueueStore()
{
	int i;
	for( i = 0; i < MAX_PACKETS_TO_QUEUE; i++ )
		if( PacketStoreCache[i].PacketState == PS_PACKET_IS_FREE )
		{
			PacketStoreCache[i].PacketState = PS_PACKET_IS_HANDED_OUT;
			return &PacketStoreCache[i];
		}
	return NULL;
}

void QueuePacket( struct TransactionLayerPacket *Packet )
{
	if( Packet->SelfIndex >= MAX_PACKETS_TO_QUEUE )
		assert( 0 );
	Packet->PacketState = PS_PACKET_IS_PENDING_SEND;
}

void QueryPacketToSend( char *OutData, int *PacketSize )
{
	int i;
	*OutData = NULL;
	*PacketSize = 0;
	for( i = 0; i < MAX_PACKETS_TO_QUEUE; i++ )
		if( PacketStoreCache[i].PacketState == PS_PACKET_IS_PENDING_SEND )
		{
			OutData = &PacketStoreCache[i].Packet[0];
			*PacketSize = PacketStoreCache[i].PacketSize;
			PacketStoreCache[i].SentPacketCounter++;
			if( PacketStoreCache[i].SentPacketCounter >= PacketStoreCache[i].SendCount ) 
				PacketStoreCache[i].PacketState = PS_PACKET_IS_SENT;
		}
}