#include "StdAfx.h"

void PrintRegisterContent( int Addr, BYTE *Data )
{
	unsigned int iData = *(unsigned int*)Data;
	if( Addr == 0x0000 )
	{
		Dprintf( DLVerbose, "\t Generic Capabilities Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 08-13) Number of lanes and func %d", GET_BITS32( iData, 8, 13 ) );
		Dprintf( DLVerbose, "\t\t 14-14) DADR len %d", GET_BIT32( iData, 14 ) );
		Dprintf( DLVerbose, "\t\t 16-23) Application type %d", GET_BITS32( iData, 16, 23 ) );
	}
	else if( Addr == 0x0001 )
	{
		Dprintf( DLVerbose, "\t Generic Capabilities Register at %d", Addr );
	}
	else if( Addr == 0x0002 )
	{
		Dprintf( DLVerbose, "\t PHY Capabilities Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 00-03) Minor rev %d", GET_BITS32( iData, 0, 3 ) );
		Dprintf( DLVerbose, "\t\t 04-05) Major rev %d", GET_BITS32( iData, 4, 5 ) );
		Dprintf( DLVerbose, "\t\t 15-15) Can Hibernate %d", GET_BIT32( iData, 15 ) );
	}
	else if( Addr == 0x0003 )
	{
		Dprintf( DLVerbose, "\t PHY Capabilities Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 32-35) N_LSS_SYN %d", GET_BITS32( iData, 0, 3 ) );
		Dprintf( DLVerbose, "\t\t 36-39) N_LSS_DIR %d", GET_BITS32( iData, 4, 7 ) );
	}
	else if( Addr == 0x0004 )
	{
		Dprintf( DLVerbose, "\t LINK/TRAN Capabilities Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 00-03) Minor rev %d", GET_BITS32( iData, 0, 3 ) );
		Dprintf( DLVerbose, "\t\t 04-05) Major rev %d", GET_BITS32( iData, 4, 5 ) );
		Dprintf( DLVerbose, "\t\t 08-15) N_FCU %d", GET_BITS32( iData, 8, 15 ) );
		Dprintf( DLVerbose, "\t\t 16-18) Device type %d", GET_BITS32( iData, 16, 18 ) );
		Dprintf( DLVerbose, "\t\t 20-31) MAX_BLKLEN %d", GET_BITS32( iData, 20, 31 ) );
	}
	else if( Addr == 0x0005 )
	{
		Dprintf( DLVerbose, "\t LINK/TRAN Capabilities Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 32-39) N_DATA_GAP %d", GET_BITS32( iData, 0, 7 ) );
	}
	else if( Addr == 0x0008 )
	{
		Dprintf( DLVerbose, "\t Generic Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 00-00) Power Control mode %d", GET_BIT32( iData, 0 ) );
		Dprintf( DLVerbose, "\t\t 08-11) Lanes %d", GET_BITS32( iData, 8, 11 ) );
	}
	else if( Addr == 0x0009 )
	{
		Dprintf( DLVerbose, "\t Generic Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 63-63) Config Complete %d", GET_BIT32( iData, 31 ) );
	}
	else if( Addr == 0x000A )
	{
		Dprintf( DLVerbose, "\t PHY Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 04-05) Major rev %d", GET_BITS32( iData, 4, 5 ) );
		Dprintf( DLVerbose, "\t\t 06-07) Speed %d", GET_BITS32( iData, 6, 7 ) );
	}
	else if( Addr == 0x000B )
	{
		Dprintf( DLVerbose, "\t PHY Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 32-35) N_LSS_SYN %d", GET_BITS32( iData, 0, 3 ) );
		Dprintf( DLVerbose, "\t\t 36-39) N_LSS_DIR %d", GET_BITS32( iData, 4, 7 ) );
	}
	else if( Addr == 0x000C )
	{
		Dprintf( DLVerbose, "\t LINK/TRAN Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 08-15) N_FCU %d", GET_BITS32( iData, 8, 15 ) );
		Dprintf( DLVerbose, "\t\t 16-17) MAX_RETRY %d", GET_BITS32( iData, 16, 17 ) );
		Dprintf( DLVerbose, "\t\t 20-31) MAX_BLK_LEN %d", GET_BITS32( iData, 20, 31 ) );
	}
	else if( Addr == 0x000D )
	{
		Dprintf( DLVerbose, "\t LINK/TRAN Setting Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 32-39) N_DATA_GAP %d", GET_BITS32( iData, 0, 7 ) );
	}
	else if( Addr == 0x0040 )
	{
		Dprintf( DLVerbose, "\t Preset Register at %d", Addr );
		Dprintf( DLVerbose, "\t\t 00-03) GN %d", GET_BITS32( iData, 0, 3 ) );
		Dprintf( DLVerbose, "\t\t 04-07) CDCP %d", GET_BITS32( iData, 4, 7 ) );
	}
	else if( Addr == 0x0041 )
	{
		Dprintf( DLVerbose, "\t Preset Register at %d", Addr );
	}
	else
		Dprintf( DLVerbose, "\t Can not parse addr %d", Addr );
}

void PrintRegisterContent( int Addr, int Size, BYTE *data )
{
	for( int i = 0 ; i < Size; i++ )
		PrintRegisterContent( Addr + i, data + i * 4 );
}