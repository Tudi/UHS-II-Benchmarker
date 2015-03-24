#include "StdAfx.h"

int DebugLevel = 0;
int EnableFileDebug = 0;
int IsFirstRun = 1;

void Dprintf( int pDebugLevel, const char* format,... )
{
	char AllAsString[MAX_FORMAT_BUFFER_SIZE];

	if( format == NULL )
		return;

	if( pDebugLevel < DebugLevel )
		return;

	va_list vlist;
	va_start( vlist, format );
	vsnprintf_s( AllAsString, MAX_FORMAT_BUFFER_SIZE / MAX_PARAM_COUNT, MAX_PARAM_COUNT, format, vlist);
	va_end( vlist );

	if( EnableFileDebug == 1 )
	{
		FILE *f;
		if( IsFirstRun == 1 )
		{
			errno_t er = fopen_s( &f, "debug.txt", "wt" );
			IsFirstRun = 0;
		}
		else
			errno_t er = fopen_s( &f, "debug.txt", "at" );
		if( f )
		{
			fprintf( f, "%s\n", AllAsString );
			fclose( f );
		}
	}
	else
		printf( "%s\n", AllAsString );
}