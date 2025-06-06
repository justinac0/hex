#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "tile.hpp"

typedef struct TileMap {
    unsigned short width;
    unsigned short height;
    Tile *tiles;

    TileMap(unsigned short width, unsigned short height);

    void Generate();
    void LoadFromFile(const char *filepath);
    Tile *GetTile(float wx, float wy); // takes world x and y
    void Draw();

    unsigned int Length();
} TileMap;

#endif