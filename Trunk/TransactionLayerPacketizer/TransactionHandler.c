#include "StdAfx.h"

struct HostTransactionStore		HostState;
struct DeviceTransactionStore	DeviceState;

void InitHostTransactionStore()
{
	EmbededMemSet( &HostState, 0, sizeof( HostState ) );
	HostState.DeviceMaxBlockLength = 512;

	EmbededMemSet( &DeviceState, 0, sizeof( DeviceState ) );
}

void InitNewHostDeviceTransaction()
{
	HostState.TransactionID++;
}