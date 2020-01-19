#ifndef __HACKS__
#define __HACKS__

#ifdef _SATURN
#include <jo/jo.h>
#endif

void* (*p_alloc)(unsigned int); /*fnptr to alloc*/
void (*p_free)(const void * const p); /* fnpointer to free*/
void (*p_panic)(char*, const char*);


#endif
