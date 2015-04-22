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
	// Device Init begin
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
		// after 30 send tries Host needs to treat init configuration as BAD
		ParsePcktCCMDDeviceInit( PacketQueueStore->PacketResponse, PacketQueueStore->PacketSizeResponse );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

#ifdef	XILINX_PROJECT_BUILD
	//seems like device will not signal us completion flag ? :O
	HostState.DeviceFinishedInitialize = 1;
#endif
	/////////////////////////////////////////
	// Device Init end
	/////////////////////////////////////////

	/////////////////////////////////////////
	// Device enum begin
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

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

		// parse the reply and in case CF is set to 0 than resend this packet until CF = 1
		// after 30 send tries Host needs to treat init configuration as BAD
		ParsePcktCCMDDeviceEnum( PacketQueueStore->PacketResponse, PacketQueueStore->PacketSizeResponse );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Device enum end
	/////////////////////////////////////////
}
