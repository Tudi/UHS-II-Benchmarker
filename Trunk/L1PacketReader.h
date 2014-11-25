#ifndef _L1_PACKET_READER_H_
#define _L1_PACKET_READER_H_

/*
Read a text file and interpret bytes as USH II Link layer command set
*/
struct sL1PacketReader
{
	FILE *File;
	char *FileName;
};

sL1PacketReader *InitL1PacketReader( char *FName );
void DestroyL1PacketReader( sL1PacketReader **PR );

#endif