#include "StdAfx.h"

int L1SymbolListSize = 0;

#ifndef USE_INTERNAL_ALLOCATOR
	L1Symbol **L1SymbolList = NULL;
#else
	#define MAX_SIMBOLS_CAN_DEFINE	50
	L1Symbol L1SymbolListStatic[ MAX_SIMBOLS_CAN_DEFINE ];
	L1Symbol *L1SymbolListListStatic[ MAX_SIMBOLS_CAN_DEFINE ];
	L1Symbol **L1SymbolList = L1SymbolListListStatic;
#endif

L1Symbol *InitSymbol()
{
#ifndef USE_INTERNAL_ALLOCATOR
	L1Symbol *ret = (L1Symbol *)EmbededMalloc( sizeof( L1Symbol ) );
#else
	assert( L1SymbolListSize < MAX_SIMBOLS_CAN_DEFINE );
	L1Symbol *ret = &L1SymbolListStatic[ L1SymbolListSize ];
#endif
	memset( ret, 0, sizeof( L1Symbol ) );
	return ret;
}

int AddSymbol( const char *Name, BYTE pS1, BYTE pS01, BYTE pS02, void (*PB)( BYTE**, int*, char* ), char* (*PP)( BYTE**, int * ), const char *Desc )
{
	Dprintf( DLVerbose, "\t\tL1 Symbol List %d: adding %s - %s", L1SymbolListSize, Name, Desc );

	L1Symbol *NS = InitSymbol();
#ifndef USE_INTERNAL_ALLOCATOR
	L1SymbolList = (L1Symbol**)realloc( L1SymbolList, ( L1SymbolListSize + 1 ) * sizeof( L1Symbol* ) );
#endif
	L1SymbolList[ L1SymbolListSize ] = NS;
	L1SymbolListSize++;

	strcpy_s( NS->Name, MAX_L1_SYMBOL_LENGTH, Name );
	strcpy_s( NS->Description, MAX_L1_SYMBOL_LENGTH, Desc );

	NS->S1 = pS1;
	NS->S0[0] = pS01;
	NS->S0[1] = pS02;

//	NS->BinaryCode = Code;
	NS->PacketBuilder = PB; 
	NS->PacketParser = PP; 

	return 0;
}

void InitL1SymbolList()
{
	Dprintf( DLVerbose, "Started L1 Symbol List intialization" );

	AddSymbol( "STB.L", STBL,		STBL,		STBL,		L1BuildPckt_STBL,	L0ParsePckt_STBL,	"Wakeup signal" );
	AddSymbol( "SYN",	LSS_COM,	LSS_SYN0,	LSS_SYN1,	L1BuildPckt_SYN,	L0ParsePckt_SYN,	"Sync for exiting STB" );
	AddSymbol( "LIDL",	LSS_COM,	LSS_LIDL0,	LSS_LIDL1,	L1BuildPckt_LIDL,	L0ParsePckt_LIDL,	"Logical idle" );
	AddSymbol( "DIDL",	LSS_COM,	LSS_DIDL0,	LSS_DIDL1,	L1BuildPckt_DIDL,	L0ParsePckt_DIDL,	"Data idle" );
	AddSymbol( "DCMD",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_DCMD,	L0ParsePckt_DCMD,	"Request read/write data start" );
	AddSymbol( "RES",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_RES,	L0ParsePckt_RES,	"command response packet" );
	AddSymbol( "DATA",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_DATA,	L0ParsePckt_DATA,	"data packet" );
//	AddSymbol( "CCMDR",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_CCMDR,	L0ParsePckt_CCMD,	"control command read" );
//	AddSymbol( "CCMDW",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_CCMDW,	L0ParsePckt_CCMD,	"control command write" );
//	AddSymbol( "FCRDY",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_FCRDY,	L0ParsePckt_FCRDY,	"signal ready read data" );
//	AddSymbol( "FCREQ",	LSS_COM,	LSS_SOP,	LSS_SOP,	L1BuildPckt_FCREQ,	L0ParsePckt_FCREQ,	"signal ready write data" );
//	AddSymbol( "STB.H", "STB.H", "STB.H",	( 2 << 0 ) | ( 2 << 8 ), L1BuildPckt_STBH, "Shutdown signal" );
//	AddSymbol( "BSYN",	"BSYN", "COMBSYN0", ( K285 << 0 ) | ( D45 << 8 ), L1BuildPckt_BSYN, "Sync for exiting STB and boot code loading");
//	AddSymbol( "DIR",	"DIR",	"COMDIR",	( K285 << 0 ) | ( D312 << 8 ), L1BuildPckt_DIR, "Direction switch" );
//	AddSymbol( "DIDL",	"DIDL", "DIDL0",	( K285 << 0 ) | ( D122 << 8 ), L1BuildPckt_DIDL, "Logical idle during data transmission" );
//	AddSymbol( "SDB",	"SDB",	"COMSDB",	( K285 << 0 ) | ( K280 << 8 ), L1BuildPckt_SDB, "Start of data burst" );
//	AddSymbol( "SOP",	"SOP",	"COMSOP",	( K285 << 0 ) | ( K281 << 8 ), L1BuildPckt_SOP, "Start of packet" );
//	AddSymbol( "EOP",	"EOP",	"COMEOP",	( K285 << 0 ) | ( K297 << 8 ), L1BuildPckt_EOP, "End of packet" );
//	AddSymbol( "EDB",	"EDB",	"COMEDB",	( K285 << 0 ) | ( K277 << 8 ), L1BuildPckt_EDB, "End of data burst" );

	Dprintf( DLVerbose, "\tEnded L1 Symbol List intialization" );
}