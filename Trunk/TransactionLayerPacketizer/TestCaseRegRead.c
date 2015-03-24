#include "StdAfx.h"

void TestCaseReadRegisterValue()
{
	char	PacketStoreBuffer[ 32767 ];
	char	PayloadStoreBuffer[ 256 ];
	int		DataUsedCounter;
	DataUsedCounter = 0;

	BuildPcktCCMD( PacketStoreBuffer, &DataUsedCounter, TRL_RW_Read, RA_Configuration, NULL, CCMD_PL_NONE );
}