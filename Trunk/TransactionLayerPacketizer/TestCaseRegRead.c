#include "StdAfx.h"

void TestCaseReadRegisterValue()
{
	struct TransactionLayerPacket *PacketQueueStore;
	char	PacketStoreBuffer[ 32767 ];
	char	PayloadStoreBuffer[ 256 ];
	int		DataUsedCounter;
	DataUsedCounter = 0;

	InitPacketQueueForNewTestCase();

	PacketQueueStore = GetPacketQueueStore();
	BuildPcktCCMD( PacketQueueStore->Packet, &PacketQueueStore->PacketSize, TRL_RW_Read, RA_Configuration, NULL, CCMD_PL_NONE );
	QueuePacket( PacketQueueStore );
}