#ifndef _L1_PACKET_SCRAMBLER_H_
#define _L1_PACKET_SCRAMBLER_H_

/*
packet data ( NOT opcode ! ) + CRC will be scrambled by given algorithm
running the scrambler second time will descramble data ( will be same as initial data )
*/
void ScramblePacket(BYTE *buf, int len);

#endif