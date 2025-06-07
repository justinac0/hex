#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <raylib.h>
#include "tile.hpp"

typedef struct TileMap {
    unsigned short width;
    unsigned short height;
    Tile *tiles;

    TileMap(unsigned short width, unsigned short height);
    ~TileMap();

    void Generate();
    void LoadFromFile(const char *filepath);
    Tile *GetTileFromWorldSpace(float wx, float wy);
    void Draw();

    unsigned int Length();
} TileMap;

#endif