#pragma once

// to make a broadcast, you need to have DID = 0 and SID = 0
// right now broadcast / ring connection is not supported. Need to add multiple device IDs ( first / last ) to be able to broadcast / comunicate with them
struct HostTransactionStore
{
	int HostID;
	int DeviceID;
	int	SessionID;
	int	TransactionID;							// can be used to ensure we are reading a packet that we were waiting the reply for. Can be used for async communication
	int DeviceFinishedInitialize;
	int DeviceFinishedEnum;
	int	DeviceMaxBlockLength,UsedBlockLength;	// by default it's 512
};

struct DeviceTransactionStore
{
	u_CFG_LINK_TRAN_REG			DeviceLinkTranReg;
	u_CFG_GENERIC_SETTINGS		DeviceGenericSettingReg;
	u_CFG_PHY_SETTINGS			DevicePHYSettingsReg;
	u_CFG_LINK_TRAN_SETTING_REG	DeviceLinkTranSettingReg;
};

extern struct HostTransactionStore		HostState;
extern struct DeviceTransactionStore	DeviceState;

void InitHostTransactionStore();
void InitNewHostDeviceTransaction();