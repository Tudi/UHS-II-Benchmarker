#ifndef _L1_SYMBOL_SET_H_
#define _L1_SYMBOL_SET_H_

#define MAX_L1_SYMBOL_LENGTH	1000

#define K277 0xFB
#define K280 0x1C
#define K281 0x3C
#define K283 0x7C
#define K285 0xBC
#define K297 0xFD
#define D045 BinToDec( 10100100 )
#define D167 BinToDec( 11110001 )
#define D122 BinToDec( 00101100 )
#define D262 BinToDec( 01011010 )
#define D312 BinToDec( 01011111 )
#define D315 BinToDec( 01011111 )

#define LSS_COM		K285
#define LSS_SOP		K281
#define LSS_EOP		K297
#define LSS_SYN0	D315
#define LSS_SYN1	D262
#define LSS_LIDL0	K283
#define LSS_LIDL1	D167

struct L1Symbol
{
	char	Name[MAX_L1_SYMBOL_LENGTH];	//as you can see it in the input file
	BYTE	S1;		//as you can see it in the document
	BYTE	S0[2];	//used for parsing 
//	BYTE	C1C0[MAX_L1_SYMBOL_LENGTH];		//not used 
//	BYTE	C1C0Scrambled[MAX_L1_SYMBOL_LENGTH];	//not used 	
//	int		BinaryCode;
//	int		Duration;	//number of times needed to be sent. Not used yet
	char	Description[MAX_L1_SYMBOL_LENGTH];	
	void	(*PacketBuilder)( BYTE **Data, int *DataLen, char *Line );
	BYTE	*(*PacketParser)( BYTE *Data );
};

extern L1Symbol **L1SymbolList;
extern int L1SymbolListSize;

void InitL1SymbolList();

#endif