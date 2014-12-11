#include "StdAfx.h"

#define CLEAR_BIT( N, BitN ) ( N & ( ~( 1 << BitN ) ) )
#define SET_BIT( N, BitN, Val ) ( CLEAR_BIT( N, BitN ) | ( Val << BitN ) )
#define GET_BIT( N, BitN ) ( ( N >> BitN ) & 0x01 )

void ScramblePacket(BYTE *buf, int len)
{
	BYTE	TempPacket[ MAX_PACKET_SIZE ];
	memcpy( TempPacket, buf, len );

/*	unsigned short	lfsra[16];
	unsigned short	lfsrb[16];

	memset( lfsra, 1, sizeof( lfsra ) );

	int LFSR		= 0xFFFF;
	int LFSROut;
	int LFSRNext;

	for( int counter = 0; counter < len; counter++)
    {
		int res = 0;
		int Data = *buf;
		for( int i = 0; i < 8; i++ )
		{
			LFSROut = LFSR >> 15;
			LFSRNext = ( LFSR << 1 ) | LFSROut;
			int LFSRNextBit3 = ( GET_BIT( LFSR, 2 ) ^ LFSROut );
			int LFSRNextBit4 = ( GET_BIT( LFSR, 3 ) ^ LFSROut );
			int LFSRNextBit5 = ( GET_BIT( LFSR, 4 ) ^ LFSROut );
			LFSRNext = SET_BIT( LFSRNext, 3, LFSRNextBit3 );
			LFSRNext = SET_BIT( LFSRNext, 4, LFSRNextBit4 );
			LFSRNext = SET_BIT( LFSRNext, 5, LFSRNextBit5 );

			LFSR = LFSRNext & 0x0000FFFF;

			res = ( res << 1 ) | ( ( Data & 0x01 ) ^ LFSROut );
//			res = ( res << 1 ) | ( ( ( Data >> ( 7 - i ) ) & 0x01 ) ^ LFSROut );
			Data = Data >> 1;
		}

		*buf = res & 0xFF;
		buf++;

		memcpy( lfsra, lfsrb, sizeof( lfsrb ) );
    } /**/

/*	int lfsr = 0xFFFF;
	for( int counter = 0; counter < len; counter++)
    {
		// polynomial: x^16 + x^5 + x^4 + x^3 + 1
		int Data = buf[counter];
		for( int i=0;i<8;i++)
		{
//			Data = Data ^ ( bit << i );
//			Data = Data ^ ( bit << ( 7 - i ) );
//			Data = Data ^ ( ( lfsr & 1 ) << i );
			unsigned int bit = ((lfsr >> 0) ^ (lfsr >> 11) ^ (lfsr >> 12) ^ (lfsr >> 13) ) & 1;
			lfsr = (lfsr >> 1) | (bit << 15);
//			Data = Data ^ ( ( lfsr & 1 ) << i );
		}
		buf[counter] = buf[counter] ^ ( lfsr & 0xFF );
    } /**/
/*
	int lfsr = 0xFFFF;
	for( int counter = 0; counter < len; counter++)
    {
		for( int j = 0; j < 8 ; j++ )
		{
			int lfsrnext = 0;
			int Bit16 = lfsr >> 15;
			for( int i=0;i<16;i++)
			{
				int NewBit;
				if( i == 0 )
					NewBit = Bit16;
				else if( i == 3 || i == 4 || i == 5 )
					NewBit = Bit16 ^ ( GET_BIT( lfsr, i - 1 ) );
				else
					NewBit = GET_BIT( lfsr, i - 1 );
				lfsrnext = SET_BIT( lfsrnext, i, NewBit );
			}
			lfsr = lfsrnext;
		}
		buf[counter] = buf[counter] ^ ( lfsr >> 8 & 0xFF );
	}/**/
/*	int lfsr = 0xFFFF;
	for( int counter = 0; counter < len; counter++)
    {
		int inverslfsr = 0;
		for( int j = 0; j < 8 ; j++ )
		{
			int Bit16 = lfsr >> 15;
			int bit3 = GET_BIT( lfsr, 3 );
			int bit4 = GET_BIT( lfsr, 4 );
			int bit5 = GET_BIT( lfsr, 5 );
			int newbit = ( Bit16 ^ bit3 ^ bit4 ^ bit5 );
			lfsr = ( ( lfsr << 1 ) | newbit ) & 0xFFFF;
			inverslfsr |= ( newbit << ( j ) );
		}
		buf[counter] = buf[counter] ^ ( inverslfsr & 0xFF );
	}/**/
	int lfsr = 0xFFFF;
	for( int counter = 0; counter < len; counter++)
    {
		// polynomial: x^16 + x^5 + x^4 + x^3 + 1
//		int Data = buf[counter];
		for( int i=0;i<8;i++)
		{
//			Data = Data ^ ( bit << i );
//			Data = Data ^ ( bit << ( 7 - i ) );
//			Data = Data ^ ( ( lfsr & 1 ) << i );
			unsigned int bit = ((lfsr >> 0) ^ (lfsr >> 11) ^ (lfsr >> 12) ^ (lfsr >> 13) ) & 1;
			lfsr = (lfsr >> 1) | (bit << 15);
//			Data = Data ^ ( ( lfsr & 1 ) << i );
		}
		buf[counter] = buf[counter] ^ ( lfsr & 0xFF );
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
	if( data[0] != 0x4E || data[1] != 0x17 || data[2] != 0xC0 || data[3] != 0x14 || data[4] != 0xB2 || data[5] != PAD || data[6] != 0xFE || data[7] != 0x1E )
		printf( "Scrambler is not working as expected!!! \n" );
*/