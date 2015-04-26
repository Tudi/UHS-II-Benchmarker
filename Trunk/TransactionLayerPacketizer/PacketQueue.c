#include "StdAfx.h"

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

	//memorize TransactionId for later packet processing
	Packet->TransactionId = HostState.TransactionID;

	//signal packet start for link layer
	Xil_Out32(TRANSM_ADDR, (unsigned int)PHY0_PACKET_HEADER0 );
	Xil_Out32(TRANSM_ADDR, (unsigned int)PHY0_PACKET_HEADER1 );

	xil_printf( "Sending Data :");
	//send actual transaction layer packet
	for( i = 0; i < Packet->PacketSize; i++ )
	{
		unsigned int FormatedData = READ_MASK | ((int)Packet->Packet[i]);
		xil_printf( " 0x%X(0x%X)", FormatedData, (int)(Packet->Packet[i]) );
		Xil_Out32( TRANSM_ADDR, FormatedData );
	}
	xil_printf( "\n");

	//signal packet end for link layer
	Xil_Out32(TRANSM_ADDR, (unsigned int)PHY0_PACKET_FOOTER0 );
	Xil_Out32(TRANSM_ADDR, (unsigned int)PHY0_PACKET_FOOTER1 );

	//initialize timer for the roundtrip of the packet
	PacketSendStartTimer( Packet );
}

void WaitDevicePacketReply( struct TransactionLayerPacket *Packet )
{
	int WaitTimeout = 0;
	unsigned int DataOnPort;

	// set it even if we do not have a reply to avoid bugs
	Packet->PacketSizeResponse = 0;

	//end transaction of send -> receive if packet does not require a reply
	// used for broadcast read CCMD packet ( SID = DID = 0 )
	if( Packet != NULL && Packet->PacketDoesNotHaveDeviceReply == 1 )
	{
		PacketRecvStopTimer( Packet );
		return;
	}

	//have to sleep. No idea why
//	SleepMS( 10 );

//xil_printf( "\n");
	//wait until we see data header
	do{
		DataOnPort = Xil_In32( RECEIV_ADDR );
		WaitTimeout = WaitTimeout + 1 ;
//xil_printf( " 0x%X", data_in0 );
	}while( WaitTimeout < MAX_WAIT_READ_PACKET_HEADER && ( DataOnPort & DATA_READ_FLAGS_NOT_DATA ) == DATA_READ_FLAGS_NOT_DATA );
//	xil_printf( "wait count : %d \n", WaitTimeout );
//xil_printf( "\n");

	//read packet content
	xil_printf( "data read : " );
	if( WaitTimeout < MAX_WAIT_READ_PACKET_HEADER )
	{
		//we recevied usable data. Stop the timer for the packet roundtrip
		if( ( DataOnPort & DATA_READ_FLAGS_IS_DATA ) == DATA_READ_FLAGS_IS_DATA )
			PacketRecvStopTimer( Packet );

		//read data until we will find packet footer in the FIFO
		do{
			if( ( DataOnPort & DATA_READ_FLAGS_IS_DATA ) == DATA_READ_FLAGS_IS_DATA && Packet->PacketSizeResponse < MaxPacketSize )
			{
				Packet->PacketResponse[ Packet->PacketSizeResponse ] = ( DataOnPort & ( ~DATA_READ_FLAGS_IS_DATA ) );
xil_printf( " 0x%X(0x%X)", DataOnPort, (int)(Packet->PacketResponse[ Packet->PacketSizeResponse ]) );
				Packet->PacketSizeResponse = Packet->PacketSizeResponse + 1;
			}
			DataOnPort = Xil_In32( RECEIV_ADDR );
		}while( ( DataOnPort & DATA_READ_FLAGS_IS_DATA ) == DATA_READ_FLAGS_IS_DATA );
	}
	else
		xil_printf( " Data read wait timeout !" );
xil_printf( "\n");
}

void WaitPhysicalLayerEmptyFifo()
{
	int WaitTimeout = 0;
	unsigned int DataOnPort;
	int FoundPacketInFiFO;

	do{
		FoundPacketInFiFO = 0;
		//wait for non data
		do{
			DataOnPort = Xil_In32( RECEIV_ADDR );
			WaitTimeout = WaitTimeout + 1 ;
	//xil_printf( " 0x%X", data_in0 );
		}while( WaitTimeout < MAX_WAIT_READ_PACKET_HEADER && ( DataOnPort & DATA_READ_FLAGS_NOT_DATA ) == DATA_READ_FLAGS_NOT_DATA );
	//	xil_printf( "wait count : %d \n", WaitTimeout );
	//xil_printf( "\n");

		//read packet content
	//	xil_printf( "data read : " );
		if( WaitTimeout < MAX_WAIT_READ_PACKET_HEADER )
		{
			//read data until we will find packet footer in the FIFO
			while( ( DataOnPort & DATA_READ_FLAGS_IS_DATA ) == DATA_READ_FLAGS_IS_DATA )
			{
				FoundPacketInFiFO = 1;
				DataOnPort = Xil_In32( RECEIV_ADDR );
			}
		}
	}while( FoundPacketInFiFO == 1 );
}
