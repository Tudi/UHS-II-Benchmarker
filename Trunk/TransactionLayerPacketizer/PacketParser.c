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
}