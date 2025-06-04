#ifndef TILE_HPP
#define TILE_HPP

#include "util.hpp"

#include <random>
#include <raylib.h>
#include <math.h>

#define TILE_RADIUS (128)

typedef struct Vec2i {
    int x;
    int y;
} Vec2i;

typedef struct Tile {
    Vec2i pos;
    Color color;

    Tile();
    Tile(Vec2i pos);
    void Draw();
    void DebugDraw();
} Tile;

#endif