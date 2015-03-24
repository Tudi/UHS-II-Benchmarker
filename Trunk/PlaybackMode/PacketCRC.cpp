#include "StdAfx.h"
  
unsigned short crc16_ccitt(const BYTE *buf, int len)
{
	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
	return crc;
}

/*
	//crc test from doc
	BYTE data[10];
	data[0] = BinToDec( 11110001 );
	data[1] = BinToDec( 0 );
	data[2] = BinToDec( 1 );
	data[3] = BinToDec( 10000000 );
	int rest = crc16_ccitt( data, 4 );
	rest	0x00004b40	int
*/