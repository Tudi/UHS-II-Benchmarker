#include "StdAfx.h"

void ScramblePacket(BYTE *buf, int len)
{
	BYTE	TempPacket[ MAX_PACKET_SIZE ];
	memcpy( TempPacket, buf, len );

	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
	{
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf)&0x00FF];
		*buf = crc & 0xFF;
		buf++;
	}

	//padding must be restored
	for( counter = 0; counter < len; counter++)
		if( TempPacket[ counter ] == PAD )
			buf[ counter ] = PAD;
}