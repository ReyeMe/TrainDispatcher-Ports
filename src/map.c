#include "../include/map.h"

/**
 * Load map data from file
 */
Tile* Map_Load(const char* file)
{
    int size;
    char* data = p_readFile(file, "MAPS", &size);
}
