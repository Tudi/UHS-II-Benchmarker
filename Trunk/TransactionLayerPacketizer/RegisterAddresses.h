#pragma once

// page 135
enum REGISTER_ADDRESS_NAMES
{
	RA_Configuration	= 0x000,
	RA_Interrupt		= 0x100,
	RA_Status			= 0x180,
	RA_Command			= 0x200,
};

// page 179
enum REGISTER_ADDRESS_NAMES_CMD
{
	RA_CMD_FULL_RESET	= 0x000,
	RA_CMD_GO_DORMANT	= 0x001,
	RA_CMD_DEVICE_INIT	= 0x002,
	RA_CMD_ENUMERATE	= 0x003,
	RA_CMD_TRANS_ABORT	= 0x004,
};

// page 167
enum REGISTER_ADDRESS_NAMES_CFG
{
	RA_CFG_GENERIC			= 0x000,
	RA_CFG_PHY_CAP			= 0x008,
	RA_CFG_LINK_TRAN		= 0x010,
	RA_CFG_RESERVED_LINK	= 0x018,
	RA_CFG_GENERIC_SETTINGS	= 0x020,
	RA_CFG_PHY_SETTINGS		= 0x028,
	RA_CFG_LINK_TRAN_SETTINS= 0x030,
	RA_CFG_RESERVED1		= 0x038,
	RA_CFG_PRESET			= 0x100,
	RA_CFG_RESERVED2		= 0x108,
};

#pragma pack(push,1)

//page 171
//read only register values
typedef union
{
	struct CFG_LINK_TRAN_REG
	{
		BYTE	LinkTranMinorRev:4,
				LinkTranMajorRev:2,
				Reserved0:2;
		BYTE	N_FCU;
		short	DeviceType:3,
				Reserved1:1,
				MaxBlkLen:12;
		BYTE	N_DATA_GAP;
		BYTE	Reserved2[3];
	}Fields;
	unsigned char	DataC[8];
}u_CFG_LINK_TRAN_REG;

// page 173
// read - write register
typedef union
{
	struct CFG_GENERIC_SETTINGS
	{
		BYTE	PowerControlMode:1,	// 0 = fast mode = filled with LIDL, 1 = LowPower = EIDL / STB / SYN 
				Reserved0:7;
		BYTE	LaneStatus:4,		// 0 = 2 lanes FD
				Reserved1:4;
		BYTE	Reserved2[2];
		BYTE	Reserved3:7,
				CompletionFlag:1;
	}Fields;
	unsigned char	DataC[8];
}u_CFG_GENERIC_SETTINGS;

// page 174
// read - write register
typedef union
{
	struct CFG_PHY_SETTINGS
	{
		BYTE	Reserved0:4,
				PHYMajorRev:2,
				TransmissionSpeed:2;	// 0 = Speed A, 1 = Speed B
		BYTE	Reserved2[3];
		BYTE	N_LSS_SYN:4,
				N_LSS_DIR:4;
		BYTE	Reserved3[3];
	}Fields;
	unsigned char	DataC[8];
}u_CFG_PHY_SETTINGS;

//page 174
//read only register values
typedef union
{
	struct CFG_LINK_TRAN_SETTING_REG
	{
		BYTE	Reserved0;
		BYTE	N_FCU;				// max number of blocks in a flow control unit, for SD, values should be : 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
									// block count * block size needs to be smaller than 64k !
		short	MAX_RETRY_NUM:2,	// For burst. Default is 3
				Reserved1:2,
				MAX_BLK_LEN:12;		// default 200h = 512 bytes
		BYTE	N_DATA_GAP;
		BYTE	Reserved2[3];
	}Fields;
	unsigned char	DataC[8];
}u_CFG_LINK_TRAN_SETTING_REG;

#pragma pack(pop)