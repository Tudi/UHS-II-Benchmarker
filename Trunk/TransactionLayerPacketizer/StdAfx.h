#pragma once

#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef NULL
	#define NULL 0
#endif


#ifndef assert
	#define assert(x) if( !x ) exit( 1 )
#endif

#include "Util.h"
#include "PacketTypes.h"
#include "PacketBuilder.h"
#include "RegisterAddresses.h"

//contains all test case headers in 1
#include "TestCases.h"