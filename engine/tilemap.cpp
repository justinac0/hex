#include "tilemap.hpp"

#include <time.h>
#include <assert.h>

TileMap::TileMap(unsigned short width, unsigned short height) {
    this->width = width;
    this->height = height;
    this->tiles = (Tile*)calloc(this->width * this->height, sizeof(Tile));
    assert(this->tiles);
}

void TileMap::Generate() {
    srand(time(NULL));
    Image noise = GenImagePerlinNoise(width, height, rand()%100, rand()%100, 5.0f);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            size_t i = (size_t)y * width + (size_t)x;
            Color* sample = GetPixelFromImage(&noise, x, y);
            this->tiles[i] = Tile({x, y});
            this->tiles[i].color = TileColorFromSample(sample);
        }
    }
}

void TileMap::LoadFromFile(const char *filepath) {

}

Tile *TileMap::GetTile(float wx, float wy) {
    for (int i = 0; i < width * height; i++) {
        if (tiles[i].ContainsPoint({wx, wy})) {
            return &tiles[i];
        }
    }

    return nullptr;
}

void TileMap::Draw() {
    for (int i = 0; i < width * height; i++) {
        tiles[i].Draw();
    }

#ifdef DEBUG
    for (int i = 0; i < width * height; i++) {
        tiles[i].DebugDraw();
    }
#endif
}

unsigned int TileMap::Length() {
    return this->width * this->height;
}