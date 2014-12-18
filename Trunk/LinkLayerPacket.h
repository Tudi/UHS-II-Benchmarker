#ifndef _LINK_LAYER_PACKET_H_
#define _LINK_LAYER_PACKET_H_

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
enum eCTG_IDX_Types
{
	CTGIDXT_FLOW_CONTROL_REQ	=	0,
	CTGIDXT_FLOW_CONTROL_READY	=	1,
	CTGIDXT_FLOW_CONTROL_STAT	=	2,
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
struct sLinkLayerPacketMSG
{
	BYTE	IDX:4,
			Reserved:1,
			CTG:3;
	BYTE	Code:8;
};
struct sLinkLayerPacketCCMD
{
	BYTE	IOADDR1:4,	//MSB
			PLEN:2,
			Reserved:1,
			ReadWrite:1;
	BYTE	IOADDR0:8;	//LSB
};
struct sLinkLayerPacketDCMD
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
};
struct sLinkLayerPacketRES
{
	BYTE	NAck:1,
			CMD_ECHO_BACK0:7;
	BYTE	CMD_ECHO_BACK1:8;
};
struct sFullLinkLayerPacketDCMD
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketDCMD	Packet;
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
struct sFullLinkLayerPacketCCMDR
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketCCMD	Packet;
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
//write packet has variable length payload
struct sFullLinkLayerPacketCCMDW0
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketCCMD	Packet;
};
struct sFullLinkLayerPacketCCMDW1
{
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
struct sFullLinkLayerPacketRES
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketRES		Packet;
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
struct sFullLinkLayerPacketDATA0
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
};
struct sFullLinkLayerPacketDATA1
{
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
struct sLinkLayerPacketCCMDDI
{
	BYTE	GAP:4,
			GD:4;
	BYTE	Reserved0:3,
			CF:1,
			DAP:4;
	BYTE	Reserved1;
	BYTE	Reserved2;
};
struct sFullLinkLayerPacketCCMDDI
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketCCMD	Packet;
	sLinkLayerPacketCCMDDI	PacketDeviceInit;
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
struct sLinkLayerPacketCCMDDE
{
	BYTE	LastNodeID:4,
			FirstNodeID:4;
	BYTE	Reserved0;
	BYTE	Reserved1;
	BYTE	Reserved2;
};
struct sFullLinkLayerPacketCCMDDE
{
	//start of the packet is sent first
	BYTE					SOPLSS[2];
	sLinkLayerPacketHeader	Header;
	sLinkLayerPacketCCMD	Packet;
	sLinkLayerPacketCCMDDE	PacketDeviceEnum;
	unsigned short			CRC;			//!!this is MSB. Most semnificative byte sent first but stored as normal int
	BYTE					EOPLSS[2];
};
#pragma pack(pop)

#endif