#ifndef _UTIL_H_
#define _UTIL_H_

/*
Some sort of hashing. Whatever
*/
int GetStrHash( const char *str );
/*
Case insensitive string compare
*/
char *stristr (char *ch1, char *ch2);
/*
Check if a file exists
*/
int FileExists( const char *fName );
/*
Reinterpret a number that is supposed to be binary into decimal
*/
BYTE BinToDec( __int64 N );
/*
Reallocate buffer and copy old content N times into the new buffer
*/
void DuplicatePacket( BYTE **Data, int *DataLen, int Count = 1, int OpcodeLocation = -1);
/*
Same as above, just the lazy man version
*/
extern int ProjectSettingOnePacketBuffer;
BYTE *DuplicatePacket( BYTE **Data, int *DataLen, char *Line, int OpcodeLocation = -1 );
/*
Words are separated by " ". Skip N words and read the specific "int"
Line consists of : Lane Command Repeat Param0 Param1...ParamN
*/
int GetLineParamXInteger( char *Line, int ParamIndex );
/*
Words are separated by " ". Skip N words and read the specific "int"
Line consists of : Lane Command Repeat Param0 Param1...ParamN
*/
int GetLineParamXHexSTR( char *Line, int ParamIndex, BYTE *Out, int MaxCount );
/*
Check if a packet is a replica of the previosu packet
*/
int CheckPacketDuplicat( BYTE *packet1, BYTE *packet2, int PacketSize, int *VariableLocations, int *VariableLocValues );
/*
Keep checking for packet duplicats
*/
int CountPacketDuplicat( BYTE **PacketList, int *AvailableBytes, int PacketSize, int *VariableLocations, int *VariableLocValues );
/*
Convert binary data to hex string
*/
char *GenericFormatPacketAsHex( BYTE *ByteStream, int ProcessedByteCount, int PacketSize, char *PacketName );
/*
Playing with BITs
*/
#define CLEAR_BIT32( N, BitN ) ( N & ( ~( 1 << BitN ) ) )
#define SET_BIT32( N, BitN, Val ) ( CLEAR_BIT32( N, BitN ) | ( Val << BitN ) )
#define GET_BIT32( N, BitN ) ( ( N >> BitN ) & 0x01 )
#define GET_BITS32( N, BitStart, BitEnd ) ( ( (unsigned int )N << ( 32 - BitEnd ) ) >> ( 32 - ( BitEnd - BitStart ) ) )
#define GET_BITS64( N, BitStart, BitEnd ) ( ( ( __int64 )N << ( 64 - BitEnd ) ) >> ( 64 - ( BitEnd - BitStart ) ) )
#define CLEAR_BITS32( N, BitStart, BitEnd ) ( ( ~( GET_BITS32( ~0, BitStart, BitEnd ) << BitStart ) & N ) )
#define SET_BITS32( N, BitStart, BitEnd, Val ) ( CLEAR_BITS32( N, BitStart, BitEnd ) | ( Val << BitStart ) )
/*
Flip Bits. Can specify the length of data block
*/
unsigned int FlipBits( unsigned int In, unsigned int BlockLenght = 8);
/*
In case Embeded library does not have one
*/
char *EmbededStrStr( char *SearchIn, char *SearchFor );

#endif