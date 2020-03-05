#ifndef __ENGINE__
#define __ENGINE__

#include "input.h"
#include "time.h"

/**
 * Initialize engine
 */
void Core_Initialize();

/**
 * Game loop
 */
void Core_GameLogic(GameTime *time);

#endif
