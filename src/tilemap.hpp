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
    void DrawCulled(Camera2D camera, int screen_width, int screen_height);

    Image perlin;

    // render region (Vec2i){x: start, y: end}
    Vec2i render_region_x, render_region_y;

    unsigned int Length();
} TileMap;

#endif