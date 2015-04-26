#include "StdAfx.h"

// note to self, read page 153 for device initialization

void TestCaseDeviceInit()
{
	struct TransactionLayerPacket	*PacketQueueStore;
	int								DeviceInitRetryCounter;

	//init the queue system for a new testcase
	InitPacketQueueForNewTestCase();

	//init host settings
	InitHostTransactionStore();

	/////////////////////////////////////////
	// Device Init begin - page 153
	/////////////////////////////////////////
#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Init\n");
#endif

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	//build packet to be sent
	BuildPcktCCMDDeviceInit( PacketQueueStore->Packet, &PacketQueueStore->PacketSize );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	while( HostState.DeviceFinishedInitialize == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
	{
		// queue the packet to be sent. Could be an async implementation in the future
		PacketQueueStore->SendCount++;
		QueuePacket( PacketQueueStore );

		//non async implementation of data send
		SendPacketToDevice( PacketQueueStore );

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

		// parse the reply and in case CF is set to 0 than resend this packet until CF = 1
		// after MAX_PACKET_RESEND_ON_NO_REPLY send tries Host needs to treat init configuration as BAD
		ParsePcktCCMDDeviceInit( PacketQueueStore->PacketResponse, PacketQueueStore->PacketSizeResponse );

		DeviceInitRetryCounter++;
	}

//	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
//		assert( 0 );

	//seems like device will not signal us completion flag ? :O
	HostState.DeviceFinishedInitialize = 1;
//	if( HostState.DeviceID == HostState.HostID )
//		HostState.DeviceID = 1;

	/////////////////////////////////////////
	// Device Init end
	/////////////////////////////////////////

	//throw remaining data from FIFO
	WaitPhysicalLayerEmptyFifo();

	/////////////////////////////////////////
	// Device enum begin - page 159 for enumeration mechanism
	/////////////////////////////////////////

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Enum\n");
#endif

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	BuildPcktCCMDDeviceEnum( PacketQueueStore->Packet, &PacketQueueStore->PacketSize );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	while( HostState.DeviceFinishedEnum == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
	{
		PacketQueueStore->SendCount++;
		QueuePacket( PacketQueueStore );

		//non async implementation of data send
		SendPacketToDevice( PacketQueueStore );

		//if the FIFO of the physical layer is not initialized, you can read bad data from it. Make sure we wait enough for the first packet to arrive in it.
		if( HostState.PhysicalLayerSendFirstPacket == 0 )
		{
			HostState.PhysicalLayerSendFirstPacket = 1;
			SleepMS( PACKET_WAIT_ARRIVE_SAFE_TIME_MS );
		}

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

		// we are waiting for device to give us he's device ID
		ParsePcktCCMDDeviceEnum( PacketQueueStore );

		DeviceInitRetryCounter++;
	}

//	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
//		assert( 0 );

	//seems like device will not signal us completion flag ? :O
	HostState.DeviceFinishedEnum = 1;

	/////////////////////////////////////////
	// Device enum end
	/////////////////////////////////////////

	//throw remaining data from FIFO
	WaitPhysicalLayerEmptyFifo();
}
