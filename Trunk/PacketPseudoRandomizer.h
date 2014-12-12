#ifndef _PACKET_PSEUDORANDOMIZER_H_
#define _PACKET_PSEUDORANDOMIZER_H_

/* 
to avoid packet opcode emulation packet opcodes should be randomized on send
A pseudo random generator without seed will generate the same output every time for the same input
for us this is good because we might need to debug the stream
*/

BYTE	GetRandomizedOpcode( BYTE opcode );

#endif
