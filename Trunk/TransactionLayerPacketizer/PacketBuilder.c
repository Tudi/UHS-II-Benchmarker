#include "StdAfx.h"

void BuildPcktCCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen )
{
	int				i;
	TLPU_CCMD		*packet;
	char			*payload;
	int				SizeOfFullHeader;
	int				RealPayloadSize;

	//sanity checks. Best case these are not used
	*OutDataLen	= 0;
	packet = (TLPU_CCMD *)OutData;
	if( packet == NULL )
		return;
	// payload is measured in 4 byte units not bytes
	assert( InPayloadLen <= CCMD_PL_MAX_PAYLOAD_LEN );

	//it's bad if we mess this up
	SizeOfFullHeader = sizeof( packet->Fields );

	//set the packet header + argument to "0". In case in the future we change the header this will ensure we set those fields to 0 also.
	assert( sizeof( packet->Fields ) == sizeof( packet->DataC ) );
	EmbededMemSet( packet->DataC, 0, SizeOfFullHeader ); 

	//set the values for the packet Header
	packet->Fields.Header.Fields.DestinationID = HostState.DeviceID;
	packet->Fields.Header.Fields.PacketType = LLPT_CCMD;
	packet->Fields.Header.Fields.NativePacket = 1;
	packet->Fields.Header.Fields.TransactionID = HostState.TransactionID;	//always 0 for non FCU packets ?
	packet->Fields.Header.Fields.Reserved = 0;
	packet->Fields.Header.Fields.SourceID = HostState.HostID;

	//set the values for the packet Argument
	packet->Fields.Argument.Fields.Reserved = 0;
	packet->Fields.Argument.Fields.PLEN = InPayloadLen;
	packet->Fields.Argument.Fields.ReadWrite = InRW;
	packet->Fields.Argument.Fields.IOADDR0 = ( InAddr >> 0 ) & 0xFF;
	packet->Fields.Argument.Fields.IOADDR1 = ( InAddr >> 8 ) & 0x0F;

	// payload size is not exactly multiple of 4....
	if( InPayloadLen == CCMD_PL_4BYTES )
		RealPayloadSize = 4;
	else if( InPayloadLen == CCMD_PL_8BYTES )
		RealPayloadSize = 8;
	else if( InPayloadLen == CCMD_PL_16BYTES )
		RealPayloadSize = 16;
	else
		RealPayloadSize = 0;

	//set the payload of the packet
	payload = &OutData[ SizeOfFullHeader ];
	if( InPayload != NULL )
	{
		for( i = 0; i < RealPayloadSize; i++ )
			payload[i] = InPayload[i];
	}
	else
	{
		for( i = 0; i < RealPayloadSize; i++ )
			payload[i] = 0;
	}

	//set the amount of bytes we should send to the link layer
	if( InRW == TRL_RW_Write )
		*OutDataLen = SizeOfFullHeader + RealPayloadSize;
	else
		*OutDataLen = SizeOfFullHeader;	// for read operation, payload is not sent only received
}

void BuildPcktCCMDDeviceInit( unsigned char *OutData, int *OutDataLen )
{
	TLPU_CCMD_PayloadDeviceInit		DeviceInitPayload;

	DeviceInitPayload.Fields.GAP = 15;
	DeviceInitPayload.Fields.GD = 0;
	DeviceInitPayload.Fields.Reserved0 = 0;
	DeviceInitPayload.Fields.CF = 1;
	DeviceInitPayload.Fields.DAP = 2;
	DeviceInitPayload.Fields.Reserved1 = 0;
	DeviceInitPayload.Fields.Reserved2 = 0;

	BuildPcktCCMD( OutData, OutDataLen, TRL_RW_Write, RA_Command + RA_CMD_DEVICE_INIT, (char*)&DeviceInitPayload, CCMD_PL_4BYTES );
}

void BuildPcktCCMDDeviceEnum( unsigned char *OutData, int *OutDataLen )
{
	TLPU_CCMD_PayloadDeviceEnum		DeviceEnumPayload;
	DeviceEnumPayload.Fields.LastNodeID = 0;
	DeviceEnumPayload.Fields.FirstNodeID = 0xF;
	DeviceEnumPayload.Fields.Reserved0 = 0;
	DeviceEnumPayload.Fields.Reserved1 = 0;
	DeviceEnumPayload.Fields.Reserved2 = 0;
	BuildPcktCCMD( OutData, OutDataLen, TRL_RW_Write, RA_Command + RA_CMD_ENUMERATE, DeviceEnumPayload.DataC, CCMD_PL_4BYTES );
}

void BuildPcktCCMDDeviceQueryReg( unsigned char *OutData, int *OutDataLen, int RegisterAddress )
{
	BuildPcktCCMD( OutData, OutDataLen, TRL_RW_Read, RegisterAddress, NULL, CCMD_PL_8BYTES );

	// This needs to be a broadcast packet ?
	// Have to test this in practice. We should be able to issue device specific query and device should respond with RES instead broadcast CCMD
/*	{
		TLPU_CCMD		*packet;
		packet = (TLPU_CCMD *)OutData;
		packet->Fields.Header.Fields.DestinationID = 0;
		packet->Fields.Header.Fields.SourceID = 0;
	}/**/
}

void BuildPcktCCMDDeviceSetReg( unsigned char *OutData, int *OutDataLen, int RegisterAddress, char *OutRegValue )
{
	BuildPcktCCMD( OutData, OutDataLen, TRL_RW_Write, RegisterAddress, OutRegValue, CCMD_PL_8BYTES );

	// This needs to be a broadcast packet ?
	// Have to test this in practice. We should be able to issue device specific query and device should respond with RES instead broadcast CCMD
/*	{
		TLPU_CCMD		*packet;
		packet = (TLPU_CCMD *)OutData;
		packet->Fields.Header.Fields.DestinationID = 0;
		packet->Fields.Header.Fields.SourceID = 0;
	}/**/
}

void BuildPcktDCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen, int DuplexMode, int LengthMode, int UnitMode, int DataMode )
{
	int				i;
	TLPU_DCMD		*packet;
	char			*payload;
	int				SizeOfFullHeader;

	//sanity checks. Best case these are not used
	*OutDataLen	= 0;
	packet = (TLPU_DCMD *)OutData;
	if( packet == NULL )
		return;

	//it's bad if we mess this up
	SizeOfFullHeader = sizeof( packet->Fields );

	//set the packet header + argument to "0". In case in the future we change the header this will ensure we set those fields to 0 also.
	assert( sizeof( packet->Fields ) == sizeof( packet->DataC ) );
	EmbededMemSet( packet->DataC, 0, SizeOfFullHeader ); 

	//set the values for the packet Header
	packet->Fields.Header.Fields.DestinationID = HostState.DeviceID;
	packet->Fields.Header.Fields.PacketType = LLPT_DCMD;
	packet->Fields.Header.Fields.NativePacket = 1;
	packet->Fields.Header.Fields.TransactionID = HostState.TransactionID;
	packet->Fields.Header.Fields.Reserved = 0;
	packet->Fields.Header.Fields.SourceID = HostState.HostID;

	//set the values for the packet Argument
	packet->Fields.Argument.Fields.Reserved0 = 0;
	packet->Fields.Argument.Fields.Reserved1 = 0;
	packet->Fields.Argument.Fields.TModeDuplexMode = DuplexMode;	// fullDuples or 2L-HalfDuplex
	packet->Fields.Argument.Fields.TModeLengthMode = LengthMode;
	packet->Fields.Argument.Fields.TModeTLUnitMode = UnitMode;
	packet->Fields.Argument.Fields.TModeDataAccessMode = DataMode;
	packet->Fields.Argument.Fields.ReadWrite = InRW;
	packet->Fields.Argument.Fields.Addr = InPayloadLen;
	packet->Fields.Argument.Fields.DataLen = InAddr;

	if( UnitMode == TLUM_BYTE_MODE )
	{
		assert( InPayloadLen <= DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN );
	}
	else
	{
		assert( InPayloadLen <= DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN * DeviceState.DeviceLinkTranSettingReg.Fields.N_FCU );
	}

	//set the payload of the packet
	payload = &OutData[ SizeOfFullHeader ];
	for( i = 0; i < InPayloadLen; i++ )
		payload[i] = InPayload[i];

	//set the amount of bytes we should send to the link layer
	*OutDataLen = SizeOfFullHeader + InPayloadLen;
}

int BuildPcktDATA( struct TransactionLayerPacket *CCMDPacket, unsigned char *OutData, int *OutDataLen )
{
	int				i;
	TLPU_DATA_A		*packet;
	char			*payload;
	int				SizeOfFullHeader;
	TLPU_DCMD		*packetCCMD;
	int				TotalBytesToSend;
	int				RemainingBytesToSend;
	int				BytesCanSendNow;

	//sanity checks. Best case these are not used
	*OutDataLen	= 0;
	packet = (TLPU_DATA_A *)OutData;
	if( packet == NULL )
		return -1;

	//it's bad if we mess this up
	SizeOfFullHeader = sizeof( packet->Fields );

	//set the packet header + argument to "0". In case in the future we change the header this will ensure we set those fields to 0 also.
	assert( sizeof( packet->Fields ) == sizeof( packet->DataC ) );
	EmbededMemSet( packet->DataC, 0, SizeOfFullHeader ); 

	packetCCMD = (TLPU_DCMD*)CCMDPacket->Packet;
	//set the values for the packet Header
	packet->Fields.Header.Fields.DestinationID = HostState.DeviceID;
	packet->Fields.Header.Fields.PacketType = LLPT_DATA;
	packet->Fields.Header.Fields.NativePacket = 1;
	packet->Fields.Header.Fields.TransactionID = HostState.TransactionID;	//always 0 for non FCU packets ?
	packet->Fields.Header.Fields.Reserved = 0;
	packet->Fields.Header.Fields.SourceID = HostState.HostID;

	payload = &OutData[ SizeOfFullHeader ];

	TotalBytesToSend = packetCCMD->Fields.Argument.Fields.DataLen;
	RemainingBytesToSend = TotalBytesToSend - CCMDPacket->TransactionBytesSent;
	BytesCanSendNow = RemainingBytesToSend;
	if( BytesCanSendNow > DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN )	//N_FCU from CFG_REG
		BytesCanSendNow = DeviceState.DeviceLinkTranSettingReg.Fields.MAX_BLK_LEN;

	for( i = CCMDPacket->TransactionBytesSent; i < CCMDPacket->TransactionBytesSent + BytesCanSendNow; i++ )
		payload[i] = CCMDPacket->Packet[ sizeof( TLPU_DCMD ) + i];

	CCMDPacket->TransactionBytesSent += BytesCanSendNow;

	*OutDataLen = SizeOfFullHeader + BytesCanSendNow;

	return ( RemainingBytesToSend - BytesCanSendNow ); 
}