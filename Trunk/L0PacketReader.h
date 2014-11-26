#ifndef _L0_PACKET_READER_H_
#define _L0_PACKET_READER_H_

/*
Read a binary file and interpret bytes as USH II physical layer communication packets
*/

#define MAX_PACKET_SIZE	65535

struct sL1PacketWriter;

struct sL0PacketReader
{
	FILE	*File;
	char	*FileName;
	BYTE	PacketBuffer[MAX_PACKET_SIZE];
	int		PacketSize;
	BYTE	PacketBufferPrevious[MAX_PACKET_SIZE];
	int		PacketSizePrev;
	int		PacketCounter;
	int		StreamBytesRead;
};

/*
Create and initialize a packet reader
*/
sL0PacketReader *InitL0PacketReader( char *FName );
/*
Read one buffer that is a valid L0 packet
*/
int ReadNextLine( sL0PacketReader *PR );
/*
process a whole file
*/
int L0PacketReaderProcessFile( sL0PacketReader *PR, sL1PacketWriter *PW );
/*
Close files and free memory used by a reader
*/
void DestroyL0PacketReader( sL0PacketReader **PR );

#define UNKNOWN_BYTES_PACKET_HANDLER_INDEX		666

char *UnkPacketParserHandler( BYTE **, int *);
char *DummyPacketParserHandler( BYTE **, int *);

#endif