#include "StdAfx.h"

void ParsePcktCCMDDeviceInit( unsigned char *InData, int InDataLen )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_CCMD_PayloadDeviceInit		*DeviceInitPayload;

	if( InDataLen == 0 )
		return;

	P_CCMD = (TLPU_CCMD*)&InData[0];
	DeviceInitPayload = (TLPU_CCMD_PayloadDeviceInit*)&InData[sizeof(P_CCMD->Fields)];

	// CF flag is set to 0 until devices finishes init
	if( DeviceInitPayload->Fields.CF == 1 )
		HostState.DeviceFinishedInitialize = 1;
}

void ParsePcktCCMDDeviceEnum( struct TransactionLayerPacket *Packet )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_RES						*P_RES;
	TLPU_CCMD_PayloadDeviceEnum		*DeviceEnumPayload;
	int								InvalidSizeDetected;

	if( Packet->PacketSizeResponse < sizeof( P_RES->Fields ) )
	{
		xil_printf( "Error : Device Enum packet is too small. Expecting %d, got %d \n", sizeof( P_RES->Fields ) + 1, Packet->PacketSizeResponse );
		return;
	}

	if( P_RES->Fields.Argument.Fields.PLEN == CCMD_PL_4BYTES )
	{
		if( Packet->PacketSizeResponse < sizeof( P_RES->Fields ) + 4 )
		{
			xil_printf( "Error : Device Enum packet is missing data ( 4 bytes of content! ) \n" );
			InvalidSizeDetected = 1;
		}
		else
			InvalidSizeDetected = 0;
	}
	else
	{
		InvalidSizeDetected = 1;
		xil_printf( "Error : Device Enum packet is missing data ( 4 bytes of content! ) \n" );
	}

	P_CCMD = (TLPU_CCMD*)&Packet->Packet[0];
	P_RES = (TLPU_RES*)&Packet->PacketResponse[0];
	DeviceEnumPayload = (TLPU_CCMD_PayloadDeviceEnum*)&Packet->PacketResponse[sizeof(P_RES->Fields)];

	if( InvalidSizeDetected != 0 )
	{
		FormatToTextCCMDResp( P_RES );
		xil_printf( "Device Enum  FirstNodeID : %d \n", DeviceEnumPayload->Fields.FirstNodeID );
		xil_printf( "Device Enum  LastNodeID : %d \n", DeviceEnumPayload->Fields.LastNodeID );
	}

	// device finished enum if DID = SID
	if( P_RES->Fields.Header.Fields.DestinationID == P_CCMD->Fields.Header.Fields.SourceID )
	{
		HostState.DeviceFinishedEnum = 1;
		//get the device ID
		if( InvalidSizeDetected == 0 )
			HostState.DeviceID = DeviceEnumPayload->Fields.FirstNodeID;
		else
		{
			HostState.DeviceID = P_RES->Fields.Header.Fields.SourceID;
			xil_printf( "Error : Device id set to 1 due to missing data \n" );
		}
	}
}

int CheckGenericResPacketHeader( struct TransactionLayerPacket *Packet )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_RES						*P_RES;
	P_CCMD = (TLPU_CCMD*)&Packet->Packet[0];
	P_RES = (TLPU_RES*)&Packet->PacketResponse[0];

	//sanity checks
	if( Packet->PacketSizeResponse < sizeof( P_RES->Fields ) )
		return -1;
	if( P_RES->Fields.Header.Fields.PacketType != LLPT_RES )
		return -2;
	if( P_RES->Fields.Header.Fields.NativePacket != 1 )
		return -3;
	if( P_RES->Fields.Header.Fields.Reserved != 0 )
		return -4;
	if( P_RES->Fields.Header.Fields.DestinationID != P_CCMD->Fields.Header.Fields.SourceID )	//destination id should be 0 ( page 191 )
		return -5;
	if( P_RES->Fields.Header.Fields.SourceID != P_CCMD->Fields.Header.Fields.DestinationID )	// source id is probably 1 
		return -6;
	if( P_RES->Fields.Header.Fields.TransactionID != P_CCMD->Fields.Header.Fields.TransactionID )
		return -7;
	if( P_RES->Fields.Argument.Fields.NACK != 0 )	//error, for some reason the packet was rejected. We can try to resend it
		return -8;
	if( P_RES->Fields.Argument.Fields.IOADDR0 != P_CCMD->Fields.Argument.Fields.IOADDR0 )
		return -9;
	if( P_RES->Fields.Argument.Fields.IOADDR1 != P_CCMD->Fields.Argument.Fields.IOADDR1 )
		return -10;
	if( P_RES->Fields.Argument.Fields.PLEN != 0 && P_CCMD->Fields.Argument.Fields.ReadWrite == TRL_RW_Read )
	{
		if( P_RES->Fields.Argument.Fields.PLEN == CCMD_PL_4BYTES && Packet->PacketSizeResponse < sizeof( P_RES->Fields ) + 4 )
			return -11;
		if( P_RES->Fields.Argument.Fields.PLEN == CCMD_PL_8BYTES && Packet->PacketSizeResponse < sizeof( P_RES->Fields ) + 8 )
			return -12;
		if( P_RES->Fields.Argument.Fields.PLEN == CCMD_PL_16BYTES && Packet->PacketSizeResponse < sizeof( P_RES->Fields ) + 16 )
			return -13;
	}

	return 0;
}

int ParsePcktCCMDDeviceRegisterQuery( struct TransactionLayerPacket *Packet, int RegisterAddress )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_RES						*P_RES;
	int								HeaderIntegrityCheck;
	P_CCMD = (TLPU_CCMD*)&Packet->Packet[0];
	P_RES = (TLPU_RES*)&Packet->PacketResponse[0];

	HeaderIntegrityCheck = CheckGenericResPacketHeader( Packet );
	if( HeaderIntegrityCheck != 0 )
	{
		xil_printf( "Error : Header integrity check failed with error code %d \n", HeaderIntegrityCheck );
		return HeaderIntegrityCheck;
	}
	if( RegisterAddress == RA_Configuration + RA_CFG_LINK_TRAN )
	{
		FlipPacketBytes( &Packet->PacketResponse[sizeof(P_RES->Fields)], GetByteLenFromPLen( P_CCMD->Fields.Argument.Fields.PLEN ) );
		EmbededMemCpy( &Packet->PacketResponse[sizeof(P_RES->Fields)], (char*)&DeviceState.DeviceLinkTranReg, sizeof( DeviceState.DeviceLinkTranReg ) );
		return 0;
	}
	else if( RegisterAddress == RA_Configuration + RA_CFG_GENERIC_SETTINGS )
	{
		FlipPacketBytes( &Packet->PacketResponse[sizeof(P_RES->Fields)], GetByteLenFromPLen( P_CCMD->Fields.Argument.Fields.PLEN ) );
		EmbededMemCpy( &Packet->PacketResponse[sizeof(P_RES->Fields)], (char*)&DeviceState.DeviceGenericSettingReg, sizeof( DeviceState.DeviceGenericSettingReg ) );
		return 0;
	}
	else if( RegisterAddress == RA_Configuration + RA_CFG_PHY_SETTINGS )
	{
		FlipPacketBytes( &Packet->PacketResponse[sizeof(P_RES->Fields)], GetByteLenFromPLen( P_CCMD->Fields.Argument.Fields.PLEN ) );
		EmbededMemCpy( &Packet->PacketResponse[sizeof(P_RES->Fields)], (char*)&DeviceState.DevicePHYSettingsReg, sizeof( DeviceState.DevicePHYSettingsReg ) );
		return 0;
	}
	else if( RegisterAddress == RA_Configuration + RA_CFG_LINK_TRAN_SETTINS )
	{
		FlipPacketBytes( &Packet->PacketResponse[sizeof(P_RES->Fields)], GetByteLenFromPLen( P_CCMD->Fields.Argument.Fields.PLEN ) );
		EmbededMemCpy( &Packet->PacketResponse[sizeof(P_RES->Fields)], (char*)&DeviceState.DeviceLinkTranSettingReg, sizeof( DeviceState.DeviceLinkTranSettingReg ) );
		return 0;
	}
	return 1;
}

int ParsePcktCCMDDeviceRegisterSet( struct TransactionLayerPacket *Packet, int RegisterAddress )
{
	return CheckGenericResPacketHeader( Packet );
}

int ParsePcktDCMDFCURes( struct TransactionLayerPacket *Packet )
{
	return CheckGenericResPacketHeader( Packet );
}
