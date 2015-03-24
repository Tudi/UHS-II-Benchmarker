#ifndef _DEBUG_H_
#define _DEBUG_H_

/*
Multi Level debugging with file logging
*/

enum enumDebugLevels
{
	DLCritical	= 1,
	DLError		= 5,
	DLDebug		= 10,
	DLVerbose	= 15,
	DLMax		= 100,
};

#define MAX_FORMAT_BUFFER_SIZE	65535
#define MAX_PARAM_COUNT			200

void Dprintf( int pDebugLevel = 0, const char* format = NULL,... );

extern int DebugLevel;
extern int EnableFileDebug;
extern int IsFirstRun;

#endif