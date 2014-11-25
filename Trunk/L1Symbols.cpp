#include "StdAfx.h"

L1Symbol **L1SymbolList = NULL;
int L1SymbolListSize = 0;

L1Symbol *InitSymbol()
{
	L1Symbol *ret = (L1Symbol *)malloc( sizeof( L1Symbol ) );
	memset( ret, 0, sizeof( L1Symbol ) );
	return ret;
}

int AddSymbol( const char *Name, const char *L1Name, const char *S1S0, int Code, const char *Desc )
{
	Dprintf( DLVerbose, "\t\tL1 Symbol List %d: adding %s - %s", L1SymbolListSize, Name, Desc );

	L1Symbol *NS = InitSymbol();
	L1SymbolList = (L1Symbol**)realloc( L1SymbolList, ( L1SymbolListSize + 1 ) * sizeof( L1Symbol* ) );
	L1SymbolList[ L1SymbolListSize ] = NS;
	L1SymbolListSize++;

	strcpy_s( NS->Name, MAX_L1_SYMBOL_LENGTH, Name );
	strcpy_s( NS->S1S0, MAX_L1_SYMBOL_LENGTH, S1S0 );
	strcpy_s( NS->Description, MAX_L1_SYMBOL_LENGTH, Desc );

	NS->BinaryCode = Code;

	return 0;
}

#define D315 BinToDec( 1011111 )
#define D45 BinToDec( 10100100 )
#define D312 BinToDec( 01011111 )
#define D167 BinToDec( 11110001 )
#define D122 BinToDec( 00101100 )
#define K280 0x1C
#define K281 0x3C
#define K297 0xFD
#define K277 0xFB
#define K285 0xBC

void InitL1SymbolList()
{
	Dprintf( DLVerbose, "Started L1 Symbol List intialization" );

	AddSymbol( "STB.L", "SYN", "COMSYN0", ( K285 << 0 ) | ( D315 << 8 ), "Sync for exiting STB" );
	AddSymbol( "", "BSYN", "COMBSYN0", ( K285 << 0 ) | ( D45 << 8 ), "Sync for exiting STB and boot code loading");
	AddSymbol( "", "DIR", "COMDIR", ( K285 << 0 ) | ( D312 << 8 ), "Direction switch" );
	AddSymbol( "", "LIDL", "COMLIDL0", ( K285 << 0 ) | ( D167 << 8 ), "Logical idle" );
	AddSymbol( "", "DIDL", "DIDL0", ( K285 << 0 ) | ( D122 << 8 ), "Logical idle during data transmission" );
	AddSymbol( "", "SDB", "COMSDB", ( K285 << 0 ) | ( K280 << 8 ), "Start of data burst" );
	AddSymbol( "", "SOP", "COMSOP", ( K285 << 0 ) | ( K281 << 8 ), "Start of packet" );
	AddSymbol( "", "EOP", "COMEOP", ( K285 << 0 ) | ( K297 << 8 ), "End of packet" );
	AddSymbol( "", "EOP", "COMEDB", ( K285 << 0 ) | ( K277 << 8 ), "End of data burst" );

	Dprintf( DLVerbose, "\tEnded L1 Symbol List intialization" );
}