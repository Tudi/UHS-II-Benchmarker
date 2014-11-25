#include "StdAfx.h"

L1Symbol **L1SymbolList = NULL;
int L1SymbolListSize = 0;

L1Symbol *InitSymbol()
{
	L1Symbol *ret = (L1Symbol *)malloc( sizeof( L1Symbol ) );
	memset( ret, 0, sizeof( L1Symbol ) );
	return ret;
}

int AddSymbol( const char *Name, const char *L1Name, const char *S1S0, int Code, void (*PB)( BYTE**, int*, char* ), const char *Desc )
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
	NS->PacketBuilder = PB; 

	return 0;
}

void InitL1SymbolList()
{
	Dprintf( DLVerbose, "Started L1 Symbol List intialization" );

	AddSymbol( "STB.L", "STB.L", "STB.L",	( 1 << 0 ) | ( 1 << 8 ), L1BuildPacketSTBL, "Wakeup signal" );
	AddSymbol( "STB.H", "STB.H", "STB.H",	( 2 << 0 ) | ( 2 << 8 ), L1BuildPacketSTBH, "Shutdown signal" );
	AddSymbol( "SYN",	"SYN",	"COMSYN0",	( K285 << 0 ) | ( D315 << 8 ), L1BuildPacketSYN, "Sync for exiting STB" );
	AddSymbol( "BSYN",	"BSYN", "COMBSYN0", ( K285 << 0 ) | ( D45 << 8 ), L1BuildPacketBSYN, "Sync for exiting STB and boot code loading");
	AddSymbol( "DIR",	"DIR",	"COMDIR",	( K285 << 0 ) | ( D312 << 8 ), L1BuildPacketDIR, "Direction switch" );
	AddSymbol( "LIDL",	"LIDL", "COMLIDL0", ( K285 << 0 ) | ( D167 << 8 ), L1BuildPacketLIDL, "Logical idle" );
	AddSymbol( "DIDL",	"DIDL", "DIDL0",	( K285 << 0 ) | ( D122 << 8 ), L1BuildPacketDIDL, "Logical idle during data transmission" );
	AddSymbol( "SDB",	"SDB",	"COMSDB",	( K285 << 0 ) | ( K280 << 8 ), L1BuildPacketSDB, "Start of data burst" );
	AddSymbol( "SOP",	"SOP",	"COMSOP",	( K285 << 0 ) | ( K281 << 8 ), L1BuildPacketSOP, "Start of packet" );
	AddSymbol( "EOP",	"EOP",	"COMEOP",	( K285 << 0 ) | ( K297 << 8 ), L1BuildPacketEOP, "End of packet" );
	AddSymbol( "EDB",	"EDB",	"COMEDB",	( K285 << 0 ) | ( K277 << 8 ), L1BuildPacketEDB, "End of data burst" );

	Dprintf( DLVerbose, "\tEnded L1 Symbol List intialization" );
}