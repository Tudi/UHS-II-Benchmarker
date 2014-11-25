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

#endif