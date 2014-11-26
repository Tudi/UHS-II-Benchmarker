#include "StdAfx.h"

char *DummyPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}

char *UnkPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	char *Ret = (char *)malloc( 100 + *AvailableBytes * 2 + 3 );
	sprintf_s( Ret, 100, "0 UnkData %04d : ", *AvailableBytes );
	for( int i=0;i<*AvailableBytes;i++)
		sprintf_s( Ret, 100 + *AvailableBytes * 2 + 3, "%s%02X", Ret, (*ReadStream)[i] );

	*ReadStream = &(*ReadStream)[ *AvailableBytes - 1 ];
	*AvailableBytes = 0;
	return Ret;
}

char *PacketParserHandlerSTBL( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != STBL )
		return NULL;

	char *Ret = (char *)malloc( 103 );
	sprintf_s( &Ret[0], 103, "0 STBL 1 : 0x%02X", RS[0] );

	*ReadStream = &RS[ 1 ];
	*AvailableBytes = *AvailableBytes - 1;
	return Ret;
}

char *PacketParserHandlerSYN( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	BYTE *RS = *ReadStream;
	if( RS[0] != LSS_COM || ( RS[1] != LSS_SYN0 && RS[1] != LSS_SYN1 ) )
		return NULL;

	char *Ret = (char *)malloc( 104 );
	sprintf_s( &Ret[0], 104, "0 SYN 1 : " );
	for( int i=0;i<2;i++)
		sprintf_s( Ret, 104, "%s%02X", Ret, RS[ i ] );

	*ReadStream = &RS[ 2 ];
	*AvailableBytes = *AvailableBytes - 2;
	return Ret;
}

char *PacketParserHandlerLIDL( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}
