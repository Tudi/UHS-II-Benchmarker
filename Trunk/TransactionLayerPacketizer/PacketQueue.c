#include "StdAfx.h"

#define MAX_PACKETS_TO_QUEUE	50
struct TransactionLayerPacket	PacketStoreCache[MAX_PACKETS_TO_QUEUE];

void InitPacketQueueForNewTestCase()
{
	int i;
	for( i = 0; i < MAX_PACKETS_TO_QUEUE; i++ )
	{
		int j;
		char *PacketQ = (char *)&PacketStoreCache[i];
		for( j = 0; j < sizeof( MAX_PACKETS_TO_QUEUE ); j++ )
			PacketQ[j] = 0;
		PacketStoreCache[i].PacketState = PS_PACKET_IS_FREE;
		PacketStoreCache[i].SelfIndex = i;
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
			PacketStoreCache[i].PacketState = PS_PACKET_IS_SENT;
			*OutData = &PacketStoreCache[i].Packet[0];
			*PacketSize = PacketStoreCache[i].PacketSize;
		}
}