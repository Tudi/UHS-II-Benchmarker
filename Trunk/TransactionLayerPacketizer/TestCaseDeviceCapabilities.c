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
//	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_LINK_TRAN );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 1;
	while( PacketWasAccepted > 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
	{
		// queue the packet to be sent. Could be an async implementation in the future
		PacketQueueStore->SendCount++;
		QueuePacket( PacketQueueStore );

		//non async implementation of data send
		SendPacketToDevice( PacketQueueStore );

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

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
//	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_GENERIC_SETTINGS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 1;
	while( PacketWasAccepted > 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
	{
		// queue the packet to be sent. Could be an async implementation in the future
		PacketQueueStore->SendCount++;
		QueuePacket( PacketQueueStore );

		//non async implementation of data send
		SendPacketToDevice( PacketQueueStore );

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

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
//	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_PHY_SETTINGS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 1;
	while( PacketWasAccepted > 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
	{
		// queue the packet to be sent. Could be an async implementation in the future
		PacketQueueStore->SendCount++;
		QueuePacket( PacketQueueStore );

		//non async implementation of data send
		SendPacketToDevice( PacketQueueStore );

		//wait for device reply
		WaitDevicePacketReply( PacketQueueStore );

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
//	InitNewHostDeviceTransaction();

	//build packet to be sent
	BuildPcktCCMDDeviceQueryReg( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, RA_Configuration + RA_CFG_LINK_TRAN_SETTINS );

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
	PacketWasAccepted = 1;
	while( PacketWasAccepted > 0 && DeviceInitRetryCounter < MAX_PACKET_RESEND_ON_NO_REPLY )
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

/*
Start Program
Started Device Init
Sending Data : 0x680(0x80) 0x600(0x0) 0x692(0x92) 0x602(0x2) 0x60F(0xF) 0x628(0x28) 0x600(0x0) 0x600(0x0)
data read : 
Sending Data : 0x680(0x80) 0x600(0x0) 0x692(0x92) 0x602(0x2) 0x60F(0xF) 0x628(0x28) 0x600(0x0) 0x600(0x0)
data read : 
Sending Data : 0x680(0x80) 0x600(0x0) 0x692(0x92) 0x602(0x2) 0x60F(0xF) 0x628(0x28) 0x600(0x0) 0x600(0x0)
data read : 
Sending Data : 0x680(0x80) 0x600(0x0) 0x692(0x92) 0x602(0x2) 0x60F(0xF) 0x628(0x28) 0x600(0x0) 0x600(0x0)
data read : 
Sending Data : 0x680(0x80) 0x600(0x0) 0x692(0x92) 0x602(0x2) 0x60F(0xF) 0x628(0x28) 0x600(0x0) 0x600(0x0)
data read : 
Started Device Enum
Sending Data : 0x681(0x81) 0x600(0x0) 0x692(0x92) 0x603(0x3) 0x6F0(0xF0) 0x600(0x0) 0x600(0x0) 0x600(0x0)
data read :  0x6A0(0xA0) 0x610(0x10) 0x612(0x12) 0x603(0x3)
Error : Device Enum packet is missing data ( 4 bytes of content! ) 
CCMD->Header->DestinationID : 0
CCMD->Header->PacketType : 2
CCMD->Header->NativePacket : 1
CCMD->Header->TransactionID : 0
CCMD->Header->Reserved : 0
CCMD->Header->SourceID : 1
CCMD->Header->DestinationID : 0
CCMD->Argument->IOADDR1 : 2
CCMD->Argument->PLEN : 1
CCMD->Argument->Reserved : 0
CCMD->Argument->ReadWrite : 0
CCMD->Argument->IOADDR0 : 3
Device Enum  FirstNodeID : 0 
Device Enum  LastNodeID : 0 
Error : Device id set to 1 due to missing data 
Started Device Capability Query
Sending Data : 0x681(0x81) 0x600(0x0) 0x620(0x20) 0x604(0x4)
data read :  0x6A0(0xA0) 0x610(0x10) 0x620(0x20) 0x604(0x4) 0x620(0x20) 0x602(0x2) 0x610(0x10) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0)
RA_CFG_LINK_TRAN -> MaxBlkLen : 512 
RA_CFG_LINK_TRAN -> N_FCU : 16 
RA_CFG_LINK_TRAN -> N_DATA_GAP : 0 
Started Device Capability Query
Sending Data : 0x681(0x81) 0x600(0x0) 0x620(0x20) 0x608(0x8)
data read :  0x6A0(0xA0) 0x610(0x10) 0x620(0x20) 0x608(0x8) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x680(0x80) 0x600(0x0) 0x600(0x0) 0x600(0x0)
Generic Settings Register -> Completion Flag : 0 
Started Device Capability Query
Sending Data : 0x681(0x81) 0x600(0x0) 0x620(0x20) 0x60A(0xA)
data read :  0x6A0(0xA0) 0x610(0x10) 0x620(0x20) 0x60A(0xA) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x612(0x12)
RA_CFG_PHY_SETTINGS -> N_LSS_DIR : 1 
RA_CFG_PHY_SETTINGS -> N_LSS_SYN : 2 
Started Device Capability Query
Sending Data : 0x681(0x81) 0x600(0x0) 0x620(0x20) 0x60C(0xC)
data read :  0x6A0(0xA0) 0x610(0x10) 0x620(0x20) 0x60C(0xC) 0x620(0x20) 0x603(0x3) 0x602(0x2) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0) 0x600(0x0)
RA_CFG_LINK_TRAN_SETTINS -> MAX_BLK_LEN : 512 
*/