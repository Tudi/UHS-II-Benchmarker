#include "StdAfx.h"

#define MAX_OPCODE_COUNT_PACKET	2

struct PacketPseudoRandomState
{
	BYTE	Opcodes[MAX_OPCODE_COUNT_PACKET];
	char	Index;
};

#define MAX_PACKET_TYPES_RANDOMIZED	3

PacketPseudoRandomState	RandomizerStates[ MAX_PACKET_TYPES_RANDOMIZED ] = { 
		{ LSS_SYN0, LSS_SYN1, 0 }, 
		{ LSS_LIDL0, LSS_LIDL1, 0 }, 
		{ LSS_DIDL0, LSS_DIDL1, 0 } };

BYTE	GetRandomizedOpcode( BYTE opcode )
{
	for( int i = 0; i < MAX_PACKET_TYPES_RANDOMIZED; i++ )
	{
		for( int j = 0; j < MAX_OPCODE_COUNT_PACKET; j++ )
			if( RandomizerStates[i].Opcodes[j] == opcode )
			{
				BYTE ret = RandomizerStates[i].Opcodes[ RandomizerStates[i].Index ];
				RandomizerStates[i].Index = ( RandomizerStates[i].Index + 1 ) % MAX_OPCODE_COUNT_PACKET;
				return ret;
			}
	}
	return opcode;
}