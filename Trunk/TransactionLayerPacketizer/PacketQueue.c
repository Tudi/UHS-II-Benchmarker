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

void SendPacketToDevice( struct TransactionLayerPacket *Packet )
{
	int i;

	//sanity check
	if( Packet->SelfIndex >= MAX_PACKETS_TO_QUEUE )
		assert( 0 );

	//in case packet is sheduled to be sent out multiple times
	Packet->SentPacketCounter++;
	if( Packet->SentPacketCounter >= Packet->SendCount ) 
		Packet->PacketState = PS_PACKET_IS_SENT;

	//signal packet start for link layer
	Xil_Out32(TRANSM_ADDR, 0x000003BC );
	Xil_Out32(TRANSM_ADDR, 0x0000033C );

	//send actual transaction layer packet
	for( i = 0; i < PacketStoreCache[i].PacketSize; i++ )
		Xil_Out32( TRANSM_ADDR, (int)Packet->Packet[i] | (int)0x00000600 );

	//signal packet end for link layer
	Xil_Out32(TRANSM_ADDR, 0x000005BC );
	Xil_Out32(TRANSM_ADDR, 0x000005FD );
}

void WaitDevicePacketReply( struct TransactionLayerPacket *Packet )
{
	//end transaction of send -> receive if packet does not require a reply
	// used for broadcast read CCMD packet ( SID = DID = 0 )
	if( Packet != NULL && Packet->PacketDoesNotHaveDeviceReply == 1 )
		return;

	//packet read is not implemented yet
}