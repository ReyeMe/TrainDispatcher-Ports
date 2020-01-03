
typedef char tile_id;

struct MapTile {
    tile_id TileID;
    char flags;
}

struct TileDescriptor {

}

TileDescriptor * descriptordata; // Allocated at runtime.