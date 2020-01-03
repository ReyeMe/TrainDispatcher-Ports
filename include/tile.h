#ifndef __TILE__
#define __TILE__

#include "platformhacks.h"

typedef char TileID;

struct MapTile {
    TileID tile_id;
    char flags;
};

struct TileDescriptor {
    char rail_dirs;
    char shadow_rail_dirs;

    short attributes;

    TileID next_form;
};

TileDescriptor* descriptordata; /* Allocated at runtime. */
// RIP map data? Guess it didn't save.

#endif
