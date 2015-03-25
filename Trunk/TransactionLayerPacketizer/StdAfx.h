#pragma once

#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef NULL
	#define NULL 0
#endif


#ifndef assert
	#ifdef _DEBUG
		#define assert(x) if( !x ) *(int*)0 = 1;
	#else
		#define assert(x) 1=1;
	#endif
#endif

#include "Util.h"
#include "PacketTypes.h"
#include "PacketBuilder.h"
#include "RegisterAddresses.h"
#include "PacketQueue.h"
#include "TransactionHandler.h"
#include "PacketParser.h"

//contains all test case headers in 1
#include "TestCases.h"