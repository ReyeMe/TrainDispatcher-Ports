#ifndef __HACKS__
#define __HACKS__

#ifdef _SATURN
#define BOOLEAN int
#define TRUE 1
#define FALSE 0
#endif

#ifdef _C99
#define BOOLEAN bool
#define TRUE true
#define FALSE false
#endif

#ifndef BOOLEAN
#define BOOLEAN int
#define TRUE 1
#define FALSE 0
#endif

#endif
