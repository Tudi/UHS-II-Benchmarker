#pragma once

struct HostTransactionStore
{
	int IDX;
};

struct DeviceTransactionStore
{
	int IDX;
};

extern struct HostTransactionStore		HostState;
extern struct DeviceTransactionStore	DeviceState;