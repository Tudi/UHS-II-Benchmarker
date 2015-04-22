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
/*
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
}*/

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