#include "StdAfx.h"

void TestCaseReadRegisterValue()
{
	struct TransactionLayerPacket *PacketQueueStore;

	//init the queue system for a new testcase
	InitPacketQueueForNewTestCase();

	//init host settings
	EmbededMemSet( (char*)&HostState, 0, sizeof( HostState ) );

	//get a packet store from queue
	PacketQueueStore = GetPacketQueueStore();
	//fill the packet with what we need
	BuildPcktCCMD( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, TRL_RW_Read, RA_Configuration, NULL, CCMD_PL_NONE );
	//queue the packet to be sent to the device
	QueuePacket( PacketQueueStore );

	//wait for the packet to arrive to the device and read the reply
	WaitDevicePacketReply();
}