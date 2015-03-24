#include "StdAfx.h"

void TestCaseReadRegisterValue()
{
	struct TransactionLayerPacket *PacketQueueStore;
	InitPacketQueueForNewTestCase();

	PacketQueueStore = GetPacketQueueStore();
	BuildPcktCCMD( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, TRL_RW_Read, RA_Configuration, NULL, CCMD_PL_NONE );
	QueuePacket( PacketQueueStore );
}