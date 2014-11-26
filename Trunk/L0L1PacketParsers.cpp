#include "StdAfx.h"

char *DummyPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	return NULL;
}

char *UnkPacketParserHandler( BYTE **ReadStream, int *AvailableBytes )
{
	if( *AvailableBytes <= 0 )
		return NULL;
	char *Ret = (char *)malloc( *AvailableBytes * 2 + 1 );
	for( int i=0;i<*AvailableBytes;i++)
		sprintf_s( &Ret[ i * 2 ], 2, "%02X", (*ReadStream)[i] );

	*ReadStream = &(*ReadStream)[ *AvailableBytes - 1 ];
	*AvailableBytes = 0;
	return Ret;
}
