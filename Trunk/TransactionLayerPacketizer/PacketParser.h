#pragma once

/*
Device init specific CCMD
*/
void ParsePcktCCMDDeviceInit( unsigned char *InData, int InDataLen );
/*
Device enum specific CCMD
*/
void ParsePcktCCMDDeviceEnum( unsigned char *InData, int InDataLen );
/*
Device register query response ( with RES and not broadcast CCMD )
*/
int ParsePcktCCMDDeviceRegisterQuery( struct TransactionLayerPacket *Packet, int RegisterAddress );
/*
Device register query response ( with RES and not broadcast CCMD )
*/
int ParsePcktCCMDDeviceRegisterSet( struct TransactionLayerPacket *Packet, int RegisterAddress );
/*
DCMD response ( with RES and not broadcast CCMD )
*/
int ParsePcktDCMDFCURes( struct TransactionLayerPacket *Packet );


