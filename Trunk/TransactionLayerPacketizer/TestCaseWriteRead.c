#include "StdAfx.h"

// page 82 for packet framing
// page 193 for flow control
void TestCaseWriteRead()
{
	struct TransactionLayerPacket	*PacketQueueStore,*PacketQueueStoreData;
	int								DeviceInitRetryCounter;
	int								PacketWasAccepted;
	int								TLUMUsed;
	int								i, FCU;

	//init the device
	if( HostState.DeviceFinishedInitialize != 1 || HostState.DeviceFinishedEnum != 1 )
		TestCaseDeviceInit();

	//read configs
	if( DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN == 0 || DeviceState.DeviceLinkTranSettingReg.Fields.N_FCU == 0 )
		TestCaseDeviceCapabilities();

	/////////////////////////////////////////
	// Write some data - page 192 - begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	InitNewHostDeviceTransaction();

	//!!! Need to set config register to signal we will be sending 1 N_FCU data packet of size 8
	// right now this is not implemented

	//build packet to be sent
	TLUMUsed = TLUM_BYTE_MODE;
	{
		char Payload[8];
		*(int*)&Payload[0] = 0x0BADBEEF;
		*(int*)&Payload[4] = 0xFEEDFACE;
		BuildPcktDCMD( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, TRL_RW_Write, 0x10, Payload, sizeof( Payload ), TM_FULL_DUPLEX, TLM_TL_PRESENT, TLUMUsed, DAM_ADDRESS );
	}

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

		// after MAX_PACKET_RESEND_ON_NO_REPLY send tries Host needs to treat init configuration as BAD
		PacketWasAccepted = ParsePcktDCMDFCURes( PacketQueueStore );

		DeviceInitRetryCounter++;
	}

	if( DeviceInitRetryCounter >= MAX_PACKET_RESEND_ON_NO_REPLY )
		assert( 0 );


	if( TLUMUsed == TLUM_BYTE_MODE )
		FCU = 1;
	else
		FCU = DeviceState.DeviceLinkTranSettingReg.Fields.N_FCU;

	//Now send the data blocks to the device
	for( i = 0; i < FCU; i++ )
	{
		PacketQueueStoreData = GetPacketQueueStore();
		//packet builder will return the number bytes / units remaining to be sent
		BuildPcktDATA( PacketQueueStore, PacketQueueStoreData->Packet, &PacketQueueStoreData->PacketSize );
		PacketQueueStoreData->PacketDoesNotHaveDeviceReply = 1;
		QueuePacket( PacketQueueStoreData );
		SendPacketToDevice( PacketQueueStoreData );
		WaitDevicePacketReply( PacketQueueStoreData );
	}

	/////////////////////////////////////////
	// Write some data - page 192 - end
	/////////////////////////////////////////

	/////////////////////////////////////////
	// Read some data - page 196 - start
	/////////////////////////////////////////
}