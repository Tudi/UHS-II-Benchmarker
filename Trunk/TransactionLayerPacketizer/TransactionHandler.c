#include "StdAfx.h"

struct HostTransactionStore		HostState;
struct DeviceTransactionStore	DeviceState;

void InitHostTransactionStore()
{
	EmbededMemSet( (char*)&HostState, 0, sizeof( HostState ) );
	HostState.DeviceMaxBlockLength = 512;

	EmbededMemSet( (char*)&DeviceState, 0, sizeof( DeviceState ) );
}

void InitNewHostDeviceTransaction()
{
	HostState.TransactionID++;
}