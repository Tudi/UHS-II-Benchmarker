#pragma once

//this is wasting memory, but we might not have a solid allocator on the embeded system
#define MaxPacketSize			64

enum PacketStates
{
	PS_PACKET_IS_FREE			= 1,
	PS_PACKET_IS_HANDED_OUT		= 2,
	PS_PACKET_IS_PENDING_SEND	= 3,
	PS_PACKET_IS_SENT			= 4,
	PS_PACKET_IS_RECEIVED		= 5,
};

struct TransactionLayerPacket
{
	int		PacketSize;
	char	Packet[MaxPacketSize];
	char	PacketResponse[MaxPacketSize];
	int		SendCount;				//number of times this packet should be sent out 
	int		SentPacketCounter;
	int		TimeStampQueue;
	int		TimeStampSent;
	int		ReplyReceived;
	int		SelfIndex;	//for debugging purpuses when not using 
	int		PacketState;
};

/*
Reset packet indexes so packets can be handed out of cache
*/
void InitPacketQueueForNewTestCase();
/*
Fetch next packet buffer that we will fill out
*/
struct TransactionLayerPacket *GetPacketQueueStore();
/*
Push back the packet in the queue
*/
void QueuePacket( struct TransactionLayerPacket *Packet );
/*
Used by Microblaze to ask for a new packet to be sent out
*/
void QueryPacketToSend( char *OutData, int *PacketSize );
