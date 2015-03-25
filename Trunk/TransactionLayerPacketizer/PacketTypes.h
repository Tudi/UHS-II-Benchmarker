#pragma once

#define HOST_DEVICE_ID		0
#define DEVICE_DEVICE_ID	1

enum eLLPTypes
{
	LLPT_CCMD	=	0,
	LLPT_DCMD	=	1,
	LLPT_RES	=	2,
	LLPT_DATA	=	3,
	LLPT_MSG	=	4,
	LLPT_MAX,
};

//see page 81 table 5-4 to clarify these
enum eMSG_IDX_Types
{
	MSG_IDX_REQ		=	0,
	MSG_IDX_READY	=	1,
	MSG_IDX_STAT	=	2,
};

enum eMSG_CTG_Types
{
	MSG_CTG_LMSG		=	0,
	MSG_CTG_INTERRUPT	=	3,
	MSG_CTG_AMSG		=	4,
};

enum eMSG_CODE_Types
{
	MSG_CODE_NO_ERROR				=	0,
	MSG_CODE_RECOVERABLE_ERROR		=	1,
	MSG_CODE_UNRECOVERABLE_ERROR	=	128,
};

#pragma pack(push,1)

struct sLinkLayerPacketHeader
{
	BYTE	DestinationID:4,
			PacketType:3,
			NativePacket:1;
	BYTE	TransactionID:3,
			Reserved:1,
			SourceID:4;
};

typedef union
{
	struct TLP_Header
	{
		BYTE	IDX:4,
				Reserved:1,
				CTG:3;
		BYTE	Code:8;
	}Fields;
	unsigned short Data;
}TLPU_Header;

enum TR_LAYER_RW_VALUES
{
	TRL_RW_Read		= 0,
	TRL_RW_Write	= 1
};

enum CCMD_PAYLOAD_LENGTH_TYPES
{
	CCMD_PL_NONE	= 0,
	CCMD_PL_4BYTES	= 1,
	CCMD_PL_8BYTES	= 2,
	CCMD_PL_16BYTES	= 3,
};

typedef union
{
	struct TLP_CCMD_A
	{
		BYTE	IOADDR1:4,	//MSB
				PLEN:2,
				Reserved:1,
				ReadWrite:1;
		BYTE	IOADDR0:8;	//LSB
	}Fields;
	unsigned short Data;
}TLPU_CCMD_A;

typedef union
{
	struct TLP_CCMD_Header
	{
		TLPU_Header		Header;
		TLPU_CCMD_A		Argument;
	}Fields;
	unsigned char	DataC[4];
}TLPU_CCMD;

typedef union
{
	struct TLP_DCMD_A
	{
		BYTE	Reserved0:3,
				TModeDuplexMode:1,
				TModeLengthMode:1,
				TModeTLUnitMode:1,
				TModeDataAccessMode:1,
				ReadWrite:1;
		BYTE	Reserved1:8;
		int		Addr:32;
		int		DataLen:32;
	}Fields;
	unsigned char	DataC[10];
}TLPU_DCMD_A;

typedef union
{
	struct TLP_DCMD_Header
	{
		TLPU_Header		Header;
		TLPU_DCMD_A		Argument;
	}Fields;
	unsigned char	DataC[12];
}TLPU_DCMD;

typedef union
{
	struct sLinkLayerPacketRES
	{
		BYTE	NAck:1,
				CMD_ECHO_BACK0:7;
		BYTE	CMD_ECHO_BACK1:8;
	}Fields;
	unsigned short Data;
}TLPU_RES;

#pragma pack(pop)
