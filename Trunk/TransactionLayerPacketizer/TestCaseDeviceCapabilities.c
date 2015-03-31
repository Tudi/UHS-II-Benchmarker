#include "StdAfx.h"

// note to self, read page 163 for device initialization

//this test case sets the 
void TestCaseDeviceCapabilities()
{
	struct TransactionLayerPacket	*PacketQueueStore;
	int								DeviceInitRetryCounter;
	int								PacketWasAccepted;

	//init the device
	if( HostState.DeviceFinishedInitialize != 1 || HostState.DeviceFinishedEnum != 1 )
		TestCaseDeviceInit();

	/////////////////////////////////////////
	// Read the value of CFG Reg MAX_BLKLEN to determine block length - page 164 - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_LINK_TRAN );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 0;
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < 30 )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_LINK_TRAN );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Read the value of CFG Reg MAX_BLKLEN to determine block length - page 164 - end
	/////////////////////////////////////////

	/////////////////////////////////////////
	// Read the value of CFG_GENERIC_SETTINGS to determine if device finished device initialization - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_GENERIC_SETTINGS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 0;
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < 30 )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_GENERIC_SETTINGS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Read the value of CFG_GENERIC_SETTINGS to determine if device finished device initialization - end
	/////////////////////////////////////////


	/////////////////////////////////////////
	// Read the value of CFG_PHY_SETTINGS to determine if device finished device initialization - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_PHY_SETTINGS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 0;
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < 30 )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_PHY_SETTINGS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Read the value of CFG_PHY_SETTINGS to determine if device finished device initialization - end
	/////////////////////////////////////////


	/////////////////////////////////////////
	// Read the value of CFG_LINK_TRAN_SETTING_REG to determine if device finished device initialization - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_LINK_TRAN_SETTINS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 0;
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < 30 )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_LINK_TRAN_SETTINS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Read the value of CFG_LINK_TRAN_SETTING_REG to determine if device finished device initialization - end
	/////////////////////////////////////////

}