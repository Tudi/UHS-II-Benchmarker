#include "StdAfx.h"

#ifdef USE_INTERNAL_ALLOCATOR

	#define PREALLOCATED_ROUND_ROBIN_BUFFER_SIZE	65535 * 10

	int	AllocatorIndex = 0;

	//the below variables are for the sake of debugging and avoiding mistakes.
	int	FreeSum = 0;
	int AllocSum = 0;

	unsigned char StaticBuffer[ PREALLOCATED_ROUND_ROBIN_BUFFER_SIZE ];

	#define MaxAllocLocationsToDebug	100
	int	DebugMemoryOverwriteCounter = 0;
	void *DebugMemoryOverwriteList[ MaxAllocLocationsToDebug ];
	int	IsDebuggerInitialized = 0;

	#define UseBoundsCheckerSize	4 * 4		//must be multiple of four 
	#define	BoundsCheckerPattern	0x0BADBEEF

int CanAllocAtThisLocationWithoutOverWrite( void *loc )
{
	if( IsDebuggerInitialized == 0 )
	{
		IsDebuggerInitialized = 1;
		memset( DebugMemoryOverwriteList, 0, sizeof( DebugMemoryOverwriteList ) );
	}
	for( int i=0; i<MaxAllocLocationsToDebug; i++ )
		if( DebugMemoryOverwriteList[ i ] <= loc && DebugMemoryOverwriteList[ i ] != 0 )
		{
			int *AllocatedMemory = (int*)DebugMemoryOverwriteList[ i ];
			int SizeOfAlloc = AllocatedMemory[-1];
			if( (char*)DebugMemoryOverwriteList[ i ] + SizeOfAlloc > loc )
				return 1;
		}
	return 0;
}

void AddAllocatedLocation( void *loc )
{
	DebugMemoryOverwriteList[ DebugMemoryOverwriteCounter ] = loc;
	DebugMemoryOverwriteCounter = ( DebugMemoryOverwriteCounter + 1 ) % MaxAllocLocationsToDebug;
}

int RemoveAllocatedLocation( void *loc )
{
	for( int i=0;i<MaxAllocLocationsToDebug; i++ )
		if( DebugMemoryOverwriteList[ i ] == loc )
		{
			DebugMemoryOverwriteList[ i ] = NULL;
			return 0;
		}
	return 1;
}

void AddBoundsChecker( void *loc, int size )
{
	int *EndOfBuffer = (int*)( (char*)loc + size );
	for( int i = 0; i < UseBoundsCheckerSize / 4; i++ )
		EndOfBuffer[i] = BoundsCheckerPattern;
}

int CheckBoundsIntegrity( void *loc, int size )
{
	int *EndOfBuffer = (int*)( (char*)loc + size );
	for( int i = 0; i < UseBoundsCheckerSize / 4; i++ )
		if( EndOfBuffer[i] != BoundsCheckerPattern )
			return 1;
	return 0;
}
#endif

void *EmbededMalloc( size_t size )
{
#ifndef USE_INTERNAL_ALLOCATOR
	return malloc( size );
#else
	//debugging related added data
	int OrigSize = size;
	size += 4;	//store the size of alloc
	size += UseBoundsCheckerSize;	//check if we managed to fuck up memory

	if( AllocatorIndex + size >= PREALLOCATED_ROUND_ROBIN_BUFFER_SIZE )
	{
		Dprintf( DLVerbose, "Allocator is looping through static buffer. Might override previous data" );
		AllocatorIndex = 0;
	}
	if( AllocSum + size - FreeSum >= PREALLOCATED_ROUND_ROBIN_BUFFER_SIZE )
	{
		Dprintf( DLError, "Allocator buffer is too small. This is very very bad. Make it larger and recompile the code" );
		return NULL;
	}

	AllocSum += size;
	int *Size = (int*)&StaticBuffer[ AllocatorIndex ];
	*Size = (int)size;
	void *ret = &StaticBuffer[ AllocatorIndex + 4 ];
	AllocatorIndex += size;

	assert( CanAllocAtThisLocationWithoutOverWrite( ret ) == 0 );
	AddAllocatedLocation( ret );
	AddBoundsChecker( ret, OrigSize );

	return ret;
#endif
}

void EmbededFree( void *pointer )
{
#ifndef USE_INTERNAL_ALLOCATOR
	free( pointer );
#else
	assert( pointer >= &StaticBuffer[ 4 ] && pointer < &StaticBuffer[ PREALLOCATED_ROUND_ROBIN_BUFFER_SIZE ] );
	int Size = ((int*)pointer)[ -1 ];
	FreeSum += Size;

	assert( RemoveAllocatedLocation( pointer ) == 0 );
	assert( CheckBoundsIntegrity( pointer, Size - UseBoundsCheckerSize - 4 ) == 0 );
#endif
}

char *EmbededStrDup( const char *instr )
{
#ifndef USE_INTERNAL_ALLOCATOR
	return _strdup( instr );
#else
	int		len = strlen( instr ) + 1;
	char	*newstr = (char*)EmbededMalloc( len + 1 );
	memcpy( newstr, instr, len );
	return newstr;
#endif
}