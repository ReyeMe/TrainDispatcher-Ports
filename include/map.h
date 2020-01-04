#ifndef __MAP__
#define __MAP__

#include "platformhacks.h"

typedef unsigned char TileID;

enum TileDir {
    TDIR_UP = 1,
    TDIR_UP_RIGHT = 2,
    TDIR_RIGHT = 4,
    TDIR_DOWN_RIGHT = 8,
    TDIR_DOWN = 16,
    TDIR_DOWN_LEFT = 32,
    TDIR_LEFT = 64,
    TDIR_UP_LEFT = 128,
};

typedef struct {
    TileID tile_id;
    char flags;
} MapTile;

typedef struct {
    char rail_dirs;
    char shadow_rail_dirs;

    short attributes;

    TileID next_form;
} TileDescriptor;

struct MapData {
    char* map_name;
    unsigned char size_x;
    unsigned char size_y;
    MapTile* tiles;

    unsigned char num_descriptors;
    TileDescriptor* descriptors;

};

#endif
