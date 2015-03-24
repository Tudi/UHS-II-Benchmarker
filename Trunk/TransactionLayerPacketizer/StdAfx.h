#pragma once

#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef assert
	#define assert(x) if( !x ) exit( 1 )
#endif

#include "Util.h"
#include "PacketTypes.h"