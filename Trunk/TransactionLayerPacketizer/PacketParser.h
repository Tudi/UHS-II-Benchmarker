#pragma once

/*
Device init specific CCMD
*/
void ParsePcktCCMDDeviceInit( unsigned char *InData, int InDataLen );
/*
Device enum specific CCMD
*/
void ParsePcktCCMDDeviceEnum( unsigned char *InData, int InDataLen );
