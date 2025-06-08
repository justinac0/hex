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
    Vector2 points[6];

    Tile();
    Tile(Vec2i pos);
    // ~Tile();

    void Draw();
    void DrawSelection();
    void DebugDraw();

    Vector2 GetWorldPosition();
    Vector2* GetWorldVertices(); // always 6 points :D
    bool ContainsPoint(Vector2 point);
} Tile;

// use for sampling noise data
static Color TileColorFromSample(Color* sample) {
    unsigned char random_sample = sample->r;

    if (random_sample > 191) {
        return WHITE;
    } else if (random_sample > 175) {
        return GRAY;
    } else if (random_sample > 127) {
        return GREEN;
    } else if (random_sample > 63) {
        return BLUE;
    } else {
        return DARKBLUE;
    }
}

#endif