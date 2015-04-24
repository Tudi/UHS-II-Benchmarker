#pragma once

/*
Read 1 line of Text from console. Special chars are skipped
*/
int ConsoleReadLine( char *Buffer, int MaxLen );
/*
set a memory block to a given value
*/
void EmbededMemSet( char *mem, char val, int size );
/*
copy a memory block to a given destination
*/
void EmbededMemCpy( char *Src, char *Dst, int count );
/*
Network packet bytes come in reversed order. Need to flip them
*/
void FlipPacketBytes( char *PacketData, int PacketLen );
/*
Convert Packet Plen type to byte length
*/
int GetByteLenFromPLen( int PLen );
/*
Safe String Copy
*/
void SafeStrCpy( char *Src, char *Dst, int MaxDestLen );

#ifndef XILINX_PROJECT_BUILD
/*
Reinterpret a number that is supposed to be binary into decimal
*/
unsigned char BinToDec( __int64 N );
/*
Required to simulate Xilinx API
*/
#define xil_printf printf
void SleepMS( int Count );
#endif