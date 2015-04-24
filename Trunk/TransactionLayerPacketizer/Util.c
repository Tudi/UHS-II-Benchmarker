#include "StdAfx.h"

int ConsoleReadLine( char *Buffer, int MaxLen )
{
	int i;
	for(i = 0; i < MaxLen; )
	{   
		int c = _getch(); 
		if( c == '\t' )
			c = ' ';
		if( c == '\r' || c == '\n' )
		{
			Buffer[i] = 0;
			printf("\n");
			return i;
		}
		else if( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c == ' ' )
			Buffer[i] = (char)c;
		else
			continue;
		printf( "%c", c );
		i++;
	}   

	return 0; 
}

void EmbededMemSet( char *mem, char val, int size )
{
	int i;
	for( i = 0; i < size; i++ )
		mem[i] = val;
}

void EmbededMemCpy( char *Src, char *Dst, int count )
{
	int i;
	for( i = 0; i < count; i++ )
		Dst[i] = Src[i];
}

void FlipPacketBytes( char *PacketData, int PacketLen )
{
	int Byte, Int;
	for( Int = 0; Int <= PacketLen - 4; Int += 4 )
		for( Byte = 0; Byte < 2; Byte++ )
		{
			int Temp = PacketData[ Int + Byte ];
			PacketData[ Int + Byte ] = PacketData[ Int + 3 - Byte ];
			PacketData[ Int + 3 - Byte ] = Temp;
		}
}

int GetByteLenFromPLen( int PLen )
{
	if( PLen == CCMD_PL_4BYTES )
		return 4;
	else if( PLen == CCMD_PL_8BYTES )
		return 8;
	else if( PLen == CCMD_PL_16BYTES )
		return 16;
	return 0;
}

#ifndef XILINX_PROJECT_BUILD
BYTE BinToDec( __int64 N )
{
	int ret = 0;
	int Index = 1;
	while( N )
	{
		assert( ( N % 10 ) == 0 || ( N % 10 ) == 1 );
		ret += ( N % 10 ) * Index;
		N = N / 10;
		Index *= 2;
	}
	assert( ret <= 255 );
	return (BYTE)ret;
}

void SleepMS( int Count )
{
	volatile int a;
	for( a = 0; a < Count; a++ );
}

#endif
