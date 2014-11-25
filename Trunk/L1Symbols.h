#ifndef _L1_SYMBOL_SET_H_
#define _L1_SYMBOL_SET_H_

#define MAX_L1_SYMBOL_LENGTH	1000

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
};

extern L1Symbol **L1SymbolList;
extern int L1SymbolListSize;

void InitL1SymbolList();

#endif