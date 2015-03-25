#include "StdAfx.h"

void BuildPcktCCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen )
{
	int				i;
	TLPU_CCMD		*packet;
	char			*payload;
	int				SizeOfFullHeader;

	//sanity checks. Best case these are not used
	*OutDataLen	= 0;
	packet = (TLPU_CCMD *)OutData;
	if( packet == NULL )
		return;

	//it's bad if we mess this up
	SizeOfFullHeader = sizeof( packet->Fields );

	//set the packet header + argument to "0". In case in the future we change the header this will ensure we set those fields to 0 also.
	assert( sizeof( packet->Fields ) == sizeof( packet->DataC ) );
	EmbededMemSet( packet->DataC, 0, SizeOfFullHeader ); 

	//set the values for the packet Header
	packet->Fields.Header.Fields.Reserved = 0;
	packet->Fields.Header.Fields.Code = 0;
	packet->Fields.Header.Fields.CTG = 0;
	packet->Fields.Header.Fields.IDX = HostState.IDX;

	//set the values for the packet Argument
	packet->Fields.Argument.Fields.Reserved = 0;
	packet->Fields.Argument.Fields.PLEN = InPayloadLen;
	packet->Fields.Argument.Fields.ReadWrite = InRW;
	packet->Fields.Argument.Fields.IOADDR0 = ( InAddr >> 0 ) & 0xFF;
	packet->Fields.Argument.Fields.IOADDR1 = ( InAddr >> 8 ) & 0x0F;

	//set the payload of the packet
	payload = &OutData[ SizeOfFullHeader ];
	for( i = 0; i < InPayloadLen; i++ )
		payload[i] = InPayload[i];

	//set the amount of bytes we should send to the link layer
	*OutDataLen = SizeOfFullHeader + InPayloadLen;
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
	packet->Fields.Header.Fields.Reserved = 0;
	packet->Fields.Header.Fields.Code = 0;
	packet->Fields.Header.Fields.CTG = 0;
	packet->Fields.Header.Fields.IDX = HostState.IDX;

	//set the values for the packet Argument
	packet->Fields.Argument.Fields.Reserved0 = 0;
	packet->Fields.Argument.Fields.Reserved1 = 0;
	packet->Fields.Argument.Fields.TModeDuplexMode = DuplexMode;
	packet->Fields.Argument.Fields.TModeLengthMode = LengthMode;
	packet->Fields.Argument.Fields.TModeTLUnitMode = UnitMode;
	packet->Fields.Argument.Fields.TModeDataAccessMode = DataMode;
	packet->Fields.Argument.Fields.ReadWrite = InRW;
	packet->Fields.Argument.Fields.Addr = InPayloadLen;
	packet->Fields.Argument.Fields.DataLen = InAddr;

	//set the payload of the packet
	payload = &OutData[ SizeOfFullHeader ];
	for( i = 0; i < InPayloadLen; i++ )
		payload[i] = InPayload[i];

	//set the amount of bytes we should send to the link layer
	*OutDataLen = SizeOfFullHeader + InPayloadLen;
}
