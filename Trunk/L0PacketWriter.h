#ifndef _L0_PACKET_WRITER_H_
#define _L0_PACKET_WRITER_H_

/*
Write a binary file that can be sent as USH II physical layer communication packets
*/

struct sL0PacketWriter
{
	FILE *File;
	char *FileName;
};

sL0PacketWriter *InitL0PacketWriter( char *FName );
void DestroyL0PacketWriter( sL0PacketWriter **PW );

#endif