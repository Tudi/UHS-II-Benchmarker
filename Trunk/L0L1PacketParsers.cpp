#include "StdAfx.h"

BYTE *DummyPacketParserHandler( BYTE *ReadStream )
{
	return ReadStream + 1;
}
