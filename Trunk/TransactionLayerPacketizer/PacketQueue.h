#pragma once

#define MAX_PACKETS_TO_QUEUE	50

//this is wasting memory, but we might not have a solid allocator on the embeded system
#define MaxPacketSize			64
#define MaxTransactionNameLen	15

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
	int				PacketSize;						// number of bytes to be sent. Not including header / footer
	unsigned char	Packet[MaxPacketSize];			// the content of the packet to be sent to physical layer. not including header + footer
	int				PacketSizeResponse;				// number of bytes received. Not including header / footer
	unsigned char	PacketResponse[MaxPacketSize];	// the content of the packet received from physical layer. not including header + footer
	int				SendCount;						// Number of times this packet should be sent out
	int				SentPacketCounter;				// Number of times the packet got sent until now
	int				TimeStampSent;					// the moment when we sent this packet to the physical layer. Timestamp is based on Microblaze timer settings !
	int				TimeStampRecv;					// the moment we received this packet from the physical layer. Timestamp is based on Microblaze timer settings !
	int				SelfIndex;						// for debugging purpuses
	int				PacketState;					// Packet state for queue manager
	int				PacketDoesNotHaveDeviceReply;	// broadcast read packets do not get a reply
	int				TransactionId;					// inherit TransactionID from session for later to be processed. This is redundancy for the sake of later data processing
	int				TransactionBytesSent;			// in byte mode data transfer this will mean the number of bytes sent
	unsigned char	TransactionName[MaxTransactionNameLen];	// Optional : add a name to a transaction. Could be used for debugging
};

extern struct TransactionLayerPacket	PacketStoreCache[MAX_PACKETS_TO_QUEUE];
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
/*
Xilinx project does not support async packet sending
*/
void SendPacketToDevice( struct TransactionLayerPacket *Packet );
/*
Using the Microblaze provided Link Layer API, wait for the reply to the last sent packet and read the reply
*/
void WaitDevicePacketReply( struct TransactionLayerPacket *Packet );
/*
Using the Microblaze provided Link Layer API, read every usable packet until FIFO is empty. Should only be used when we expect duplicate packets that can be thrown away
*/
void WaitPhysicalLayerEmptyFifo( );
