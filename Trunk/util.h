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
int	BinToDec( int N );
/*
Reallocate buffer and copy old content N times into the new buffer
*/
BYTE *DuplicatePacket( BYTE **Data, int *DataLen, int Count = 1 );
/*
Same as above, just the lazy man version
*/
BYTE *DuplicatePacket( BYTE **Data, int *DataLen, char *Line );
/*
Words are separated by " ". Skip N words and read the specific "int"
Line consists of : Lane Command Repeat Param0 Param1...ParamN
*/
int GetLineParamXInteger( char *Line, int ParamIndex );


#endif