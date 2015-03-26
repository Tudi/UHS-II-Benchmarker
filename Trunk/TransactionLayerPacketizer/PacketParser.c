#include "StdAfx.h"

void ParsePcktCCMDDeviceInit( unsigned char *InData, int InDataLen )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_CCMD_PayloadDeviceInit		*DeviceInitPayload;

	P_CCMD = (TLPU_CCMD*)&InData[0];
	DeviceInitPayload = (TLPU_CCMD_PayloadDeviceInit*)&InData[sizeof(P_CCMD->Fields)];

	// CF flag is set to 0 until devices finishes init
	if( DeviceInitPayload->Fields.CF == 1 )
		HostState.DeviceFinishedInitialize = 1;
}

void ParsePcktCCMDDeviceEnum( unsigned char *InData, int InDataLen )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_CCMD_PayloadDeviceEnum		*DeviceEnumPayload;

	P_CCMD = (TLPU_CCMD*)&InData[0];
	DeviceEnumPayload = (TLPU_CCMD_PayloadDeviceEnum*)&InData[sizeof(P_CCMD->Fields)];

	// device finished enum if DID = SID
	if( P_CCMD->Fields.Header.Fields.DestinationID == P_CCMD->Fields.Header.Fields.SourceID )
		HostState.DeviceFinishedEnum = 1;

	//get the device ID
	HostState.DeviceID = DeviceEnumPayload->Fields.FirstNodeID;
}

void ParsePcktCCMDDeviceRegisterQuery( struct TransactionLayerPacket *Packet, int RegisterAddress )
{
	TLPU_CCMD						*P_CCMD;
	TLPU_RES						*P_RES;
	P_CCMD = (TLPU_CCMD*)&Packet->Packet[0];
	P_RES = (TLPU_RES*)&Packet->PacketResponse[0];

	//sanity checks
	if( P_RES->Fields.Header.Fields.PacketType != LLPT_RES )
		return;
	if( P_RES->Fields.Header.Fields.NativePacket != 1 )
		return;
	if( P_RES->Fields.Header.Fields.Reserved != 0 )
		return;
	if( P_RES->Fields.Header.Fields.DestinationID != P_CCMD->Fields.Header.Fields.DestinationID )
		return;
	if( P_RES->Fields.Header.Fields.SourceID != P_CCMD->Fields.Header.Fields.SourceID )
		return;
	if( P_RES->Fields.Header.Fields.TransactionID != P_CCMD->Fields.Header.Fields.TransactionID )
		return;

	if( RegisterAddress == RA_Configuration + RA_CFG_LINK_TRAN )
	{
		EmbededMemCpy( &Packet->PacketResponse[sizeof(P_RES->Fields)], (char*)&DeviceState.DeviceLinkTranReg, sizeof( DeviceState.DeviceLinkTranReg ) );
	}
}
