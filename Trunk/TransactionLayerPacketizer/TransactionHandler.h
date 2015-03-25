#pragma once

// to make a broadcast, you need to have DID = 0 and SID = 0
struct HostTransactionStore
{
	int HostID;
	int DeviceID;
	int	SessionID;
	int	TransactionID;
	int DeviceFinishedInitialize;
	int DeviceFinishedEnum;
};

struct DeviceTransactionStore
{
	int None;
};

extern struct HostTransactionStore		HostState;
extern struct DeviceTransactionStore	DeviceState;