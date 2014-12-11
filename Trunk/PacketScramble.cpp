#include "StdAfx.h"

unsigned char lfsr_ucShiftData(unsigned int &ulLfsrData)
{
	unsigned char ucTempBit;
	ucTempBit = (ulLfsrData & 0x0001); //get last bit
	ulLfsrData = ulLfsrData>>1; //shift
	ulLfsrData = ulLfsrData | (ucTempBit<<15); // set msb = last bit
	if(ucTempBit==1u)//check if xor mask needs to be applied
	{
		ulLfsrData = ulLfsrData ^ 0x1c00;//0001 1100 0000 0000 = 0x1c00
	}
	else
	{
		// xor with 0 = identity function - do nothing
	}
	return ucTempBit;
}

unsigned char lfsr_gucGetChar(unsigned int &ulLfsrData)
{
	unsigned char ucReturn = 0u;
	for(unsigned char ucI=0;ucI<8;ucI++)
		ucReturn = ucReturn | (lfsr_ucShiftData( ulLfsrData )<<ucI);
	return ucReturn;
}

void ScramblePacket(BYTE *buf, int len)
{
	BYTE	TempPacket[ MAX_PACKET_SIZE ];
	memcpy( TempPacket, buf, len );

	unsigned int ulLfsrData = 0x0000FFFFul;

	for( int counter = 0; counter < len; counter++)
    {
		int scrambler = lfsr_gucGetChar( ulLfsrData );
//printf("%0X ",scrambler);
		buf[counter] = buf[counter] ^ ( scrambler & 0xFF );
    } /**/

	//padding must be restored
	for( int counter = 0; counter < len; counter++)
		if( TempPacket[ counter ] == PAD )
			buf[ counter ] = PAD;
}

/*
	//scramble test from doc
	BYTE data[10];
	data[0] = 0xB1;
	data[1] = 0x00;
	data[2] = 0xAA;
	data[3] = 0xBB;
	data[4] = 0xCC;
	data[5] = PAD;
	*(unsigned short*)&data[6] = CRC_LSB_SWAP( crc16_ccitt( data, 6 ) );
	if( data[6] != 0xFE || data[7] != 0x1E )
		printf( "CRC generation and write failed!\n");
	ScramblePacket( data, 8 );
	if( data[0] != 0x4E || data[1] != 0x17 || data[2] != 0x6A || data[3] != 0xAF || data[4] != 0x7E || data[5] != PAD || data[6] != 0xFC || data[7] != 0x9C )
		printf( "Scrambler is not working as expected!!! \n" );
*/