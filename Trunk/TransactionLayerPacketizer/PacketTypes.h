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

enum eRES_CODE_Types
{
	RES_CODE_NO_ERROR		= 0,	
	RES_CODE_COND_ERROR		= 1,	// Command issued does nto respect the standard
	RES_CODE_ARG_ERROR		= 2,	// either Argument, Extended argument, or payload contains invalid value
	RES_CODE_GENERIC_ERR	= 3,	// unknown error type
};

#pragma pack(push,1)
typedef union
{
	struct TLP_Header
	{
		BYTE	DestinationID:4,
				PacketType:3,
				NativePacket:1;		// NP = 1
		BYTE	TransactionID:3,
				Reserved:1,
				SourceID:4;
	}Fields;
	unsigned char	DataC[2];
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
	CCMD_PL_MAX_PAYLOAD_LEN	 = 3
};

typedef union
{
	struct TLP_CCMD_A
	{
		BYTE		IOADDR1:4,	//MSB
					PLEN:2,
					Reserved:1,
					ReadWrite:1;
		BYTE		IOADDR0:8;	//LSB
	}Fields;
	unsigned char	DataC[2];
}TLPU_CCMD_A;

typedef union
{
	struct TLP_CCMD_Header
	{
		TLPU_Header		Header;
		TLPU_CCMD_A		Argument;
	}Fields;
	unsigned char		DataC[4];
}TLPU_CCMD;

typedef union
{
	// page 153
	struct TLP_CCMD_PayloadDeviceInit
	{
		BYTE		GAP:4,		// Group allocated power 1=360mw, 2=720mw....n=n*360mw
					GD:4;		// Group descriptor
		BYTE		Reserved0:3,
					CF:1,		// Completion flag. Device clears this flag to 0 until he is initialized. Host needs to resend device init packet until this flag remains 1
					DAP:4;		// Device allocated power n=n*360mw
		BYTE		Reserved1;
		BYTE		Reserved2;
	}Fields;
	unsigned char	DataC[4];
}TLPU_CCMD_PayloadDeviceInit;

typedef union
{
	// page 159
	struct TLP_CCMD_PayloadDeviceEnum
	{
		BYTE		LastNodeID:4,		// 
					FirstNodeID:4;		// 
		BYTE		Reserved0;
		BYTE		Reserved1;
		BYTE		Reserved2;
	}Fields;
	unsigned char	DataC[4];
}TLPU_CCMD_PayloadDeviceEnum;

enum DAMModes
{
	DAM_ADDRESS	= 0,
	DAM_PORT	= 1,
};

enum TLUModes
{
	TLUM_BLOCK_MODE	= 0,
	TLUM_BYTE_MODE	= 1,
};

enum TLModes
{
	TLM_TL_NOT_PRESENT	= 0,
	TLM_TL_PRESENT	= 1,
};

enum TModes
{
	TM_FULL_DUPLEX		= 0,
	TM_HALF_DUPLEX_2L	= 1,
};

// !! this is not good. Need to remake this
typedef union
{
	struct TLP_DCMD_A
	{
		BYTE		Reserved0:3,
					TModeDataAccessMode:1,	// DAM=0 -> increase address by 1 unit ( byte / block ), DAM=1 -> write to the same addr
					TModeTLUnitMode:1,		// byte or block ? 0 = block mode, 1 = byte mode. if TLU=1 than TL=1. if DM=1 than TLU=0
					TModeLengthMode:1,		// is tlen present in the packet or not ? 0 = not specified, 1 = specified
					TModeDuplexMode:1,		// full duplex or half duplex ? 0 = FD, 1 = 2L-HD, 
					ReadWrite:1;
		BYTE		Reserved1:8;
		int			Addr:32;				// DAM=0 -> addr is measured in blocks. Transmited MSB first, LSB last
		int			DataLen:32;				// if LM=0 this field is a data field !, TLUM=0 ( max 4GByte addr ), TLUM=1 than max TLEN is equal to 1 block size
	}Fields;
	unsigned char	DataC[10];
}TLPU_DCMD_A;

// ex page 192
typedef union
{
	struct TLP_DCMD_Header
	{
		TLPU_Header		Header;
		TLPU_DCMD_A		Argument;
	}Fields;
	unsigned char		DataC[12];
}TLPU_DCMD;

typedef union
{
	struct TLP_DATA_Header
	{
		TLPU_Header		Header;
	}Fields;
	unsigned char	DataC[2];
}TLPU_DATA_A;

typedef union
{
	struct TLP_RES_A
	{
		BYTE		NAck:1,
					Plen:7;
		BYTE		CMD_ECHO_BACK1:8;
	}Fields;
	unsigned char	DataC[2];
}TLPU_RES_A;

typedef union
{
	struct TLP_RES_A_CCMD
	{
		BYTE		IOADDR1:4,	//MSB
					PLEN:2,
					Reserved:1,
					NACK:1;		// 0 = CCMD was accepted
		BYTE		IOADDR0:8;	//LSB
	}Fields;
	unsigned char	DataC[2];
}TLPU_RES_A_CCMD;

// page 144 for example
typedef union
{
	struct TLPU_RES
	{
		TLPU_Header		Header;
		TLPU_RES_A_CCMD	Argument;
	}Fields;
	unsigned char		DataC[12];
}TLPU_RES;

#pragma pack(pop)
