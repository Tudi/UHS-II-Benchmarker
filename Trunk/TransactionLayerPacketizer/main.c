#include "StdAfx.h"
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <io.h>

int main( int argc, char *argv[] )
{
	int RunTestCase;
	if( argc == 0 )
	{
		printf( "UHS II protocol benchmarker\n");
		printf( "This project is ment to be a library.\n");
		return 0;
	}

	if( argv[1] != NULL )
		RunTestCase = atoi( argv[1] );
	else
		RunTestCase = 0;

	return 0;
}