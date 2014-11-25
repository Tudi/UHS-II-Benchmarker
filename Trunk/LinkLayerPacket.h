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
	int	DestinationID:4;
	int	PacketType:3;
	int NativePacket:1;
	int	TransactionID:3;
	int	Reserved:1;
	int SourceID:4;
};
struct sLinkLayerPacketMSG
{
	int	IDX:4;
	int	Reserved:1;
	int CTG:3;
	int	Code:8;
};
struct sLinkLayerPacketCCMD
{
	int	IOADDR0:4;
	int	PLEN:2;
	int Reserved:1;
	int	ReadWrite:1;
	int	IOADDR1:8;
};
struct sLinkLayerPacketDCMD
{
	int	Reserved0:3;
	int	TMode:4;
	int	ReadWrite:1;
	int	Reserved1:8;
	int	Addr:32;
	int	DataLen:32;
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
#pragma pack(pop)

/*
Create and initialize a command reader
*/
//sLinkLayerPacket *InitLLP( char *FName );

#endif