#include "StdAfx.h"

void TestCaseDeviceSetRegister()
{
	struct TransactionLayerPacket	*PacketQueueStore;
	int								DeviceInitRetryCounter;
	int								PacketWasAccepted;

	//init the device
	if( HostState.DeviceFinishedInitialize != 1 || HostState.DeviceFinishedEnum != 1 )
		TestCaseDeviceInit();

	//read configs
	if( HostState.DeviceMaxBlockLength = 0 )
		TestCaseDeviceCapabilities();

	/////////////////////////////////////////
	// Write the value of CFG Reg MAX_BLKLEN to determine block length - page 164 - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//build packet to be sent
	{
		u_CFG_LINK_TRAN_REG RegValue;
		EmbededMemCpy( RegValue.DataC, DeviceState.DeviceLinkTranReg.DataC, sizeof( DeviceState.DeviceLinkTranReg.DataC ) );
		RegValue.Fields.MaxBlkLen = 64;	//test value ! Change it later
		BuildPcktCCMDDeviceSetReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_GENERIC_SETTINGS, DeviceState.DeviceLinkTranReg.DataC );
	}

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
		ParsePcktCCMDDeviceRegisterSet( PacketQueueStore, RA_Configuration + RA_CFG_GENERIC_SETTINGS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Write the value of CFG Reg MAX_BLKLEN to determine block length - page 164 - end
	/////////////////////////////////////////


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
}