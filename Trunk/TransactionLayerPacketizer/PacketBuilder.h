#pragma once

/*
Construct a CCMD packet
*/
void BuildPcktCCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen );

/*
Device init specific CCMD
*/
void BuildPcktCCMDDeviceInit( unsigned char *OutData, int *OutDataLen );

/*
Device enum specific CCMD
*/
void BuildPcktCCMDDeviceEnum( unsigned char *OutData, int *OutDataLen );

/*
Device register read specific CCMD
*/
void BuildPcktCCMDDeviceQueryReg( unsigned char *OutData, int *OutDataLen, int RegisterAddress );

/*
Device register write specific CCMD
*/
void BuildPcktCCMDDeviceSetReg( unsigned char *OutData, int *OutDataLen, int RegisterAddress, char *OutRegValue );

/*
Construct a DCMD packet
*/
void BuildPcktDCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen, int DuplexMode, int LengthMode, int UnitMode, int DataMode );

/*
Construct a DCMD packet
*/
void BuildPcktDCMD( unsigned char *OutData, int *OutDataLen, int InRW, int InAddr, char *InPayload, int InPayloadLen, int DuplexMode, int LengthMode, int UnitMode, int DataMode );
