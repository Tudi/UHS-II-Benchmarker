#include "StdAfx.h"

void BuildPcktCCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen )
{
	int i;
	struct sLinkLayerPacketCCMD *packet;
	char						*payload;
	*OutDataLen = 0;
	packet = (struct sLinkLayerPacketCCMD *)OutData;
	if( packet == NULL )
		return;
	packet->Reserved = 0;
	packet->PLEN = InPayloadLen;
	packet->ReadWrite = InRW;
	packet->IOADDR0 = ( InAddr >> 0 ) & 0xFF;
	packet->IOADDR1 = ( InAddr >> 8 ) & 0x0F;
	payload = &OutData[ sizeof( struct sLinkLayerPacketCCMD ) ];
	for( i = 0; i < InPayloadLen; i++ )
		payload[i] = InPayload[i];
	*OutDataLen = sizeof( struct sLinkLayerPacketCCMD ) + InPayloadLen;
}


void BuildPcktDCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen, int DuplexMode, int LengthMode, int UnitMode, int DataMode )
{
	int i;
	struct sLinkLayerPacketDCMD *packet;
	char						*payload;
	*OutDataLen = 0;
	packet = (struct sLinkLayerPacketDCMD *)OutData;
	if( packet == NULL )
		return;
	packet->Reserved0 = 0;
	packet->Reserved1 = 0;
	packet->TModeDuplexMode = DuplexMode;
	packet->TModeLengthMode = LengthMode;
	packet->TModeTLUnitMode = UnitMode;
	packet->TModeDataAccessMode = DataMode;
	packet->ReadWrite = InRW;
	packet->Addr = InPayloadLen;
	packet->DataLen = InAddr;
	payload = &OutData[ sizeof( struct sLinkLayerPacketCCMD ) ];
	for( i = 0; i < InPayloadLen; i++ )
		payload[i] = InPayload[i];
	*OutDataLen = sizeof( struct sLinkLayerPacketDCMD ) + InPayloadLen;
}
