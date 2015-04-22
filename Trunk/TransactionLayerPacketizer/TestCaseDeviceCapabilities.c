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
#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Capability Query\n");
#endif

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
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_LINK_TRAN );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
		assert( 0 );

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "RA_CFG_LINK_TRAN -> MaxBlkLen : %d \n", DeviceState.DeviceLinkTranReg.Fields.MaxBlkLen );
	xil_printf( "RA_CFG_LINK_TRAN -> N_FCU : %d \n", DeviceState.DeviceLinkTranReg.Fields.N_FCU );
	xil_printf( "RA_CFG_LINK_TRAN -> N_DATA_GAP : %d \n", DeviceState.DeviceLinkTranReg.Fields.N_DATA_GAP );
#endif

	/////////////////////////////////////////
	// Read the value of CFG Reg MAX_BLKLEN to determine block length - page 164 - end
	/////////////////////////////////////////

	/////////////////////////////////////////
	// Read the value of CFG_GENERIC_SETTINGS to determine if device finished device initialization - begin
	/////////////////////////////////////////
#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Capability Query\n");
#endif

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
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_GENERIC_SETTINGS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
		assert( 0 );

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Generic Settings Register -> Completion Flag : %d \n", DeviceState.DeviceGenericSettingReg.Fields.CompletionFlag );
#endif

	/////////////////////////////////////////
	// Read the value of CFG_GENERIC_SETTINGS to determine if device finished device initialization - end
	/////////////////////////////////////////


	/////////////////////////////////////////
	// Read the value of CFG_PHY_SETTINGS to determine if device finished device initialization - begin
	/////////////////////////////////////////

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Capability Query\n");
#endif

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
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_PHY_SETTINGS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
		assert( 0 );

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "RA_CFG_PHY_SETTINGS -> N_LSS_DIR : %d \n", DeviceState.DevicePHYSettingsReg.Fields.N_LSS_DIR );
	xil_printf( "RA_CFG_PHY_SETTINGS -> N_LSS_SYN : %d \n", DeviceState.DevicePHYSettingsReg.Fields.N_LSS_SYN );
#endif

	/////////////////////////////////////////
	// Read the value of CFG_PHY_SETTINGS to determine if device finished device initialization - end
	/////////////////////////////////////////


	/////////////////////////////////////////
	// Read the value of CFG_LINK_TRAN_SETTING_REG to determine if device finished device initialization - begin
	/////////////////////////////////////////

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "Started Device Capability Query\n");
#endif

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
	while( PacketWasAccepted == 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
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
		PacketWasAccepted = ParsePcktCCMDDeviceRegisterQuery( PacketQueueStore, RA_Configuration + RA_CFG_LINK_TRAN_SETTINS );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
		assert( 0 );

#ifdef	XILINX_PROJECT_BUILD
	xil_printf( "RA_CFG_LINK_TRAN_SETTINS -> MAX_BLK_LEN : %d \n", DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN );
#endif

	/////////////////////////////////////////
	// Read the value of CFG_LINK_TRAN_SETTING_REG to determine if device finished device initialization - end
	/////////////////////////////////////////

}
