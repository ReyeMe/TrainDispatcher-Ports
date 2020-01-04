#ifndef __HACKS__
#define __HACKS__

#ifdef _SATURN
#define bool Bool
#endif

void* (*p_alloc)(unsigned int); /*fnptr to alloc*/
void (*p_free)(void *); /* fnpointer to free*/
void (*p_panic)(char*, char*);


#endif
