#ifndef _STDAFX_H_
#define _STDAFX_H_

//#define EMBEDED_BUILD
#ifndef EMBEDED_BUILD
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include <stdarg.h>
	#include <io.h>
	#include <assert.h>
	#include <string.h>
#endif

//this was added because embeded API does not support malloc. !!! Current implementation does not support more than 1 reader / writer and has limited memory allocation
//#define USE_INTERNAL_ALLOCATOR

#include "EmbededMalloc.h"

typedef unsigned char BYTE;

#include "util.h"
#include "debug.h"
#include "PacketCRC.h"
#include "PacketScramble.h"
#include "PacketPseudoRandomizer.h"
#include "RegisterDataFormat.h"
#include "L0PacketReader.h"
#include "L0PacketWriter.h"
#include "L1PacketReader.h"
#include "L1PacketWriter.h"
#include "L1Symbols.h"
#include "LinkLayerPacket.h"

#endif