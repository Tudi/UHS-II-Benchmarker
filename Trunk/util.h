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
BYTE *DuplicatePacket( BYTE **Data, int *DataLen, int Count = 1, int OpcodeLocation = -1);
/*
Same as above, just the lazy man version
*/
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

#endif