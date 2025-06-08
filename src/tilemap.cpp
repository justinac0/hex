#include "tilemap.hpp"

#include <time.h>
#include <assert.h>

TileMap::TileMap(unsigned short width, unsigned short height) {
    this->width = width;
    this->height = height;
    this->tiles = (Tile*)calloc(this->width * this->height, sizeof(Tile));
    assert(this->tiles);
}

TileMap::~TileMap() {
    UnloadImage(perlin);
    free(tiles);
}

void TileMap::Generate() {
    srand(time(NULL));
    perlin = GenImagePerlinNoise(width, height, rand()%100, rand()%100, width/24.0f);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            size_t i = (size_t)y * width + (size_t)x;
            Color* sample = GetPixelFromImage(&perlin, x, y);
            this->tiles[i] = Tile({x, y});
            this->tiles[i].color = TileColorFromSample(sample);
        }
    }
}

void TileMap::LoadFromFile(const char *filepath) {

}

Tile *TileMap::GetTileFromWorldSpace(float wx, float wy) {
    for (int y = this->render_region_y.x; y < this->render_region_y.y; y++) {
        for (int x = this->render_region_x.x; x < this->render_region_x.y; x++) {
            int offset = y * this->width + x;
            if (offset < 0 || offset > this->width * this->height) {
                continue;
            }

            if (tiles[offset].ContainsPoint({wx, wy})) {
                return &tiles[offset];
            }
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

void TileMap::DrawCulled(Camera2D camera, int screen_width, int screen_height) {
    // abitary size for renderable region
    float width = 2*TILE_RADIUS+2*TILE_RADIUS*cosf(PI/3);
    float height = 4*TILE_RADIUS*sinf(PI/3);

    // define screen bounds
    Vector2 tl = {0, 0};
    tl = GetScreenToWorld2D(tl, camera);

    Vector2 br = {(float)screen_width, (float)screen_height};
    br = GetScreenToWorld2D(br, camera);

    // screen bounds to indexes
    const int x_start = (int)2*(tl.x + 2*TILE_RADIUS*cosf(PI/3))/width;
    const int x_end = (int)2*(br.x + 2*TILE_RADIUS*cosf(PI/3))/width;

    const int y_start = (int)2*(tl.y + 2*TILE_RADIUS*sinf(PI/3))/height;
    const int y_end = (int)2*(br.y + 2*TILE_RADIUS*sinf(PI/3))/height;

    this->render_region_x = {x_start, x_end};
    this->render_region_y = {y_start, y_end};

    // render with some buffer to hide loading tiles
    const int buffer = 2;
    for (int y = y_start - buffer; y < y_end + buffer; y++) {
        for (int x = x_start - buffer; x < x_end + buffer; x++) {
            int offset = y * this->width + x;
            if (offset < 0 || offset > this->width * this->height) {
                continue;
            }

            this->tiles[offset].Draw();
        }
    }

    #ifdef DEBUG
    for (int y = y_start - buffer; y < y_end + buffer; y++) {
        for (int x = x_start - buffer; x < x_end + buffer; x++) {
            int offset = y * this->width + x;
            if (offset < 0 || offset > this->width * this->height) {
                continue;
            }

            this->tiles[offset].DebugDraw();
        }
    }
    #endif
}

unsigned int TileMap::Length() {
    return this->width * this->height;
}