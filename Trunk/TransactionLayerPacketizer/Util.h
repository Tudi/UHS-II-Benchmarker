#pragma once

/*
Read 1 line of Text from console. Special chars are skipped
*/
int ConsoleReadLine( char *Buffer, int MaxLen );
/*
Reinterpret a number that is supposed to be binary into decimal
*/
unsigned char BinToDec( __int64 N );