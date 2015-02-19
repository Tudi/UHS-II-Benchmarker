#ifndef PHY_INCLUDE_H
#define PHY_INCLUDE_H

typedef union
{
	/*
	9 --- CNFG_LOCK_MARGIN, CNFG_LOCK_PERIOD, CNFG_ALIGN_EN, RCLKTRMEN, RCLKOE, DET_EN, BUSIF16, HOST_MODE, CT, MODE, TDRM, TDM --- 31
	*/
	struct
	{
		unsigned int Reserved:9;
		unsigned int CNFG_LOCK_MARGIN:2;
		unsigned int CNFG_LOCK_PERIOD:2;
		unsigned int CNFG_ALIGN_EN:1;
		unsigned int RCLKTRMEN:1;
		unsigned int RCLKOE:1;
		unsigned int DET_EN:1;
		unsigned int BUSIF16:1;
		unsigned int HOST_MODE:1;
		unsigned int CT:8;
		unsigned int MODE:1;
		unsigned int TDRM:2;
		unsigned int TDM:2;
	}fields;
	unsigned int uint32_Data;
}phy_cmd_type0;

typedef union
{
	/*
	9 --- CNFG_LOCK_MARGIN, CNFG_LOCK_PERIOD, CNFG_ALIGN_EN, RCLKTRMEN, RCLKOE, DET_EN, BUSIF16, HOST_MODE, CT, MODE, TDRM, TDM --- 31
	*/
	struct
	{
		unsigned int Reserved:9;
		unsigned int CNFG_LOCK_MARGIN:2;
		unsigned int CNFG_LOCK_PERIOD:2;
		unsigned int CNFG_ALIGN_EN:1;
		unsigned int RCLKTRMEN:1;
		unsigned int RCLKOE:1;
		unsigned int DET_EN:1;
		unsigned int BUSIF16:1;
		unsigned int HOST_MODE:1;
		unsigned int CT_PHY_CMD:4;
		unsigned int CT_Tx:2;
		unsigned int CT_Rx:2;
		unsigned int MODE:1;
		unsigned int TDRM:2;
		unsigned int TDM:2;
	}fields;
	unsigned int uint32_Data;
}phy_cmd_type;

typedef union
{
	struct
	{
		unsigned int Amplitude:1;
		unsigned int Lock:1;
		unsigned int Pack:1;
		unsigned int Err:1;
		unsigned int RDS:2;
		unsigned int RDTS:2;
	}fields;
	unsigned char uint8_Data;
}STField;

typedef union
{
	struct
	{
		unsigned int RDTS:2;
		unsigned int RDS:2;
		unsigned int Err:1;
		unsigned int Pack:1;
		unsigned int Lock:1;
		unsigned int Amplitude:1;
	}fields;
	unsigned char uint8_Data;
}STField2;

typedef union
{
	struct
	{
		unsigned int b0:20;
		unsigned int ST:8;
		unsigned int RDTM:2;
		unsigned int RDM:2;
	}fields;
	unsigned int uint32_Data;
}StatusRegOnRead;

typedef union
{
	struct
	{
		unsigned char com;
		unsigned char syn;
	}fields;
	unsigned short uint16_Data;
}SynPacket;

#define K237 0xF7
#define K277 0xFB
#define K280 0x1C
#define K281 0x3C
#define K283 0x7C
#define K285 0xBC
#define K286 0xDC
#define K297 0xFD

#define LSS_COM		K285
#define LSS_SOP		K281
#define LSS_EOP		K297
#define LSS_SYN0	0xBF
#define LSS_SYN1	D262
#define LSS_LIDL0	K283
#define LSS_LIDL1	D167
#define LSS_DIDL0	K286
#define LSS_DIDL1	D122
#define PAD			K237

#endif
