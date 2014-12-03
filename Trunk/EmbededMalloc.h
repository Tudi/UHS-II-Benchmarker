#ifndef _EMBEDED_MALLOC_H_
#define _EMBEDED_MALLOC_H_

#ifndef USE_INTERNAL_ALLOCATOR
	#include <malloc.h>
#endif

void *EmbededMalloc( size_t );
void EmbededFree( void * );

char *EmbededStrDup( const char * );

#endif