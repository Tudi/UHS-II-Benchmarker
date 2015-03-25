#include "StdAfx.h"

// note to self, read page 153 for device initialization

void TestCaseReproduceDanielExample()
{
	struct TransactionLayerPacket	*PacketQueueStore;
	TLPU_CCMD						*P_CCMD;
	unsigned char					*Payload;
	int								DeviceInitRetryCounter;
	TLPU_CCMD_PayloadDeviceInit		*DeviceInitPayload;
	TLPU_CCMD_PayloadDeviceEnum		*DeviceEnumPayload;

	//init the queue system for a new testcase
	InitPacketQueueForNewTestCase();

	//init host settings
	EmbededMemSet( (char*)&HostState, 0, sizeof( HostState ) );

	/////////////////////////////////////////
	// Device Init begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	*(int*)&PacketQueueStore->Packet[0] = 0x80;
	*(int*)&PacketQueueStore->Packet[1] = 0x00;
	*(int*)&PacketQueueStore->Packet[2] = 0x92;
	*(int*)&PacketQueueStore->Packet[3] = 0x02;
	*(int*)&PacketQueueStore->Packet[4] = 0x0D;
	*(int*)&PacketQueueStore->Packet[5] = 0x28;
	*(int*)&PacketQueueStore->Packet[6] = 0x00;
	*(int*)&PacketQueueStore->Packet[7] = 0x00;

	P_CCMD = (TLPU_CCMD*)&PacketQueueStore->Packet[0];
	Payload = (unsigned char*)&PacketQueueStore->Packet[ sizeof( P_CCMD->Fields ) ];
	DeviceInitPayload = (TLPU_CCMD_PayloadDeviceInit*)Payload;
	// packet type CCMD = 0
	// source + destination ID needs to be 0
	// native packet = 1
	// plen needs to be 1
	// reserved needs to be 0
	// RW should be 1
	// DAP needs to be 2 at the begginning
	// GAP is 13, is maximum power that can be allocated to the group. Needs to be greater than the CDCP of the system !
	// IOADDR : 2 << 8 + 2 = 514 ? But why ?

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
RESEND_DEVICE_INIT_PACKET:
	PacketQueueStore->SendCount++;
	QueuePacket( PacketQueueStore );

	//wait for device reply
	WaitDevicePacketReply();

	// parse the reply and in case CF is set to 0 than resend this packet until CF = 1
	// after 30 send tries Host needs to treat init configuration as BAD
	ParsePcktCCMDDeviceInit( &PacketQueueStore->PacketResponse[0], PacketQueueStore->PacketSizeResponse );

	DeviceInitRetryCounter++;
	if( HostState.DeviceFinishedInitialize == 0 && DeviceInitRetryCounter < 30 )
		goto RESEND_DEVICE_INIT_PACKET;

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Device Init end
	/////////////////////////////////////////

	/////////////////////////////////////////
	// Device enum begin
	/////////////////////////////////////////

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();

	*(int*)&PacketQueueStore->Packet[0] = 0x80;
	*(int*)&PacketQueueStore->Packet[1] = 0x00;
	*(int*)&PacketQueueStore->Packet[2] = 0x92;
	*(int*)&PacketQueueStore->Packet[3] = 0x03;
	*(int*)&PacketQueueStore->Packet[4] = 0xF0;
	*(int*)&PacketQueueStore->Packet[5] = 0x00;
	*(int*)&PacketQueueStore->Packet[6] = 0x00;
	*(int*)&PacketQueueStore->Packet[7] = 0x00;

	P_CCMD = (TLPU_CCMD*)&PacketQueueStore->Packet[0];
	Payload = (unsigned char*)&PacketQueueStore->Packet[ sizeof( P_CCMD->Fields ) ];
	DeviceEnumPayload = (TLPU_CCMD_PayloadDeviceEnum*)Payload;
	// packet type CCMD = 0
	// source + destination ID needs to be 0
	// native packet = 1
	// plen needs to be 1
	// reserved needs to be 0
	// RW should be 1
	// firstnode should be 15, lastnode should be 0
	// IOADDR : 2 << 8 + 2 = 514 ? But why ?

	//queue the packet to be sent to the device
	PacketQueueStore->SendCount = 0;
	DeviceInitRetryCounter = 0;
RESEND_DEVICE_ENUM_PACKET:
	PacketQueueStore->SendCount++;
	QueuePacket( PacketQueueStore );

	//wait for device reply
	WaitDevicePacketReply();

	// parse the reply and in case CF is set to 0 than resend this packet until CF = 1
	// after 30 send tries Host needs to treat init configuration as BAD
	ParsePcktCCMDDeviceEnum( &PacketQueueStore->PacketResponse[0], PacketQueueStore->PacketSizeResponse );

	DeviceInitRetryCounter++;
	if( HostState.DeviceFinishedEnum == 0 && DeviceInitRetryCounter < 30 )
		goto RESEND_DEVICE_ENUM_PACKET;

	if( DeviceInitRetryCounter >= 30 )
		assert( 0 );

	/////////////////////////////////////////
	// Device enum end
	/////////////////////////////////////////

	//wait for the packet to arrive to the device and read the reply
	WaitDevicePacketReply();

}