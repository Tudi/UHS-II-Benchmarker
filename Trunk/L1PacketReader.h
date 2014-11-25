#ifndef _L1_PACKET_READER_H_
#define _L1_PACKET_READER_H_

/*
Read a text file and interpret bytes as USH II Link layer command set
*/

#define MAX_READER_LINE_BUFFER_LENGTH	65535	//should be more than enough. Expected value is less than 80

struct sL0PacketWriter;

struct sL1PacketReader
{
	FILE	*File;
	char	*FileName;
	char	LineBuffer[MAX_READER_LINE_BUFFER_LENGTH];
	int		LineCounter;
};

/*
Create and initialize a command reader
*/
sL1PacketReader *InitL1PacketReader( char *FName );
/*
Read one line that is a valid L1 command. Skips comments
*/
int ReadNextLine( sL1PacketReader *PR );
/*
process a whole file
*/
int ProcessFile( sL1PacketReader *PR, sL0PacketWriter *PW );
/*
Close files and free memory used by a reader
*/
void DestroyL1PacketReader( sL1PacketReader **PR );

#endif