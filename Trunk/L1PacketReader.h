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
Check if a string can be interpreted by us as a valid packet command 
*/
int IsValidL1Symbol( char *Line );
/*
Check if a string can be interpreted by us as a valid packet command 
*/
int IsValidL1SymbolEmbeded( char *Line );
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
int L1PacketReaderProcessFile( sL1PacketReader *PR, sL0PacketWriter *PW );
/*
Close files and free memory used by a reader
*/
void DestroyL1PacketReader( sL1PacketReader **PR );

/*
Packet builders
*/
void	L1BuildPckt_STBL( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_STBH( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_SYN( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_BSYN( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_DIR( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_LIDL( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_DIDL( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_SDB( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_SOP( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_EOP( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_EDB( BYTE **Data, int *DataLen, char *Line );
//read PDF page 140
void	L1BuildPckt_DCMD( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_RES( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_DATA( BYTE **Data, int *DataLen, char *Line );
//read PDF 153
void	L1BuildPckt_CCMDDI( BYTE **Data, int *DataLen, char *Line );
//read PDF 159
void	L1BuildPckt_CCMDDE( BYTE **Data, int *DataLen, char *Line );
//read PDF 167
void	L1BuildPckt_GETSETREG( BYTE **Data, int *DataLen, char *Line );
// PDF 81
void	L1BuildPckt_FCRDY( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_FCREQ( BYTE **Data, int *DataLen, char *Line );
void	L1BuildPckt_STAT( BYTE **Data, int *DataLen, char *Line );

#endif