#ifndef __MAP__
#define __MAP__

#include "platformhacks.h"

#define MAP_TILE_NAME_LENGTH 24
#define MAP_SIZE 101

/**
 * Rail types
 */
typedef enum {
    Rail_Empty = 0,
    Rail_Track,
    Rail_Station,
    Rail_Signal,
    Rail_Depo
} Map_RailType;

/**
 * Rail types
 */
typedef enum {
    Rail_0deg = 0,
    Rail_90deg,
    Rail_180deg,
    Rail_270deg
} Map_RailRotation;

/**
 * Station data
 */
typedef struct {
    char name[MAP_TILE_NAME_LENGTH];
} Rail_Station;

/**
 * Signal data
 */
typedef struct {
    bool active;
} Rail_Signal;

/**
 * Depo data
 */
typedef struct {
    char name[MAP_TILE_NAME_LENGTH];
} Rail_Depo;

/**
 * Basic rail
 */
typedef struct Rail{
    unsigned char code;
    Map_RailRotation rotation;
    Map_RailType type;
    struct Rail* nextForm;
    void* additionalData;
} Rail;

/**
 * Basic map tile
 */
typedef struct {
    Rail *rail;
    char level;
} Tile;

/**
 * Load map data from file
 * @return Loaded map data
 */
Tile* Map_Load(const char* file);

#endif
