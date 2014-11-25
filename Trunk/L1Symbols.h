#ifndef _L1_SYMBOL_SET_H_
#define _L1_SYMBOL_SET_H_

#define MAX_L1_SYMBOL_LENGTH	1000

#define K280 0x1C
#define K281 0x3C
#define K297 0xFD
#define K277 0xFB
#define K285 0xBC
#define D315 BinToDec( 1011111 )
#define D45 BinToDec( 10100100 )
#define D312 BinToDec( 01011111 )
#define D167 BinToDec( 11110001 )
#define D122 BinToDec( 00101100 )

#define LSS_COM	K285
#define LSS_SOP	K281
#define LSS_EOP	K297

struct L1Symbol
{
	char	Name[MAX_L1_SYMBOL_LENGTH];	//as you can see it in the input file
	char	S1S0[MAX_L1_SYMBOL_LENGTH];	//as you can see it in the document
	char	S1S0_2[MAX_L1_SYMBOL_LENGTH];	//not used 
	char	C1C0[MAX_L1_SYMBOL_LENGTH];		//not used 
	char	C1C0Scrambled[MAX_L1_SYMBOL_LENGTH];	//not used 	
	int		BinaryCode;
	int		Duration;	//number of times needed to be sent. Not used yet
	char	Description[MAX_L1_SYMBOL_LENGTH];	
	void	(*PacketBuilder)( BYTE **Data, int *DataLen, char *Line );
};

extern L1Symbol **L1SymbolList;
extern int L1SymbolListSize;

void InitL1SymbolList();

#endif