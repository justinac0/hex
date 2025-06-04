#include "tile.hpp"

Tile::Tile() {
    this->pos = {0,0};
    int n = rand() % ARRAY_SIZE(colors, Color);
    this->color = colors[n];
}

Tile::Tile(Vec2i pos) {
    this->pos = pos;
    int n = rand() % ARRAY_SIZE(colors, Color);
    this->color = colors[n];
}

void Tile::Draw() {
    float tessellation_width = TILE_RADIUS + TILE_RADIUS * 0.5; // r + r*cos(pi/3); the axis-aligned width + the slanted side width
    float tessellation_height = TILE_RADIUS * sinf(PI/3) * 2; // r*sin(pi/3)*2; the height slanted side height times 2;
    bool is_odd_column = (this->pos.x % 2);
    float x_offset = this->pos.x * tessellation_width;
    float y_offset = this->pos.y * tessellation_height + (is_odd_column * tessellation_height * 0.5);

    DrawPoly({x_offset, y_offset}, 6, TILE_RADIUS, 0, this->color);
}

void Tile::DebugDraw() {
#define TILE_TEXT_SIZE (48)
#define TILE_TEXT_PADDING (8)
    float tessellation_width = TILE_RADIUS + TILE_RADIUS * 0.5; // r + r*cos(pi/3); the axis-aligned width + the slanted side width
    float tessellation_height = TILE_RADIUS * sinf(PI/3) * 2; // r*sin(pi/3)*2; the height slanted side height times 2;
    bool is_odd_column = (this->pos.x % 2);
    float x_offset = this->pos.x * tessellation_width;
    float y_offset = this->pos.y * tessellation_height + (is_odd_column * tessellation_height * 0.5);

    // tile position to string
    char buff[256];
    int nbytes = sprintf(buff, "%d, %d", pos.x, pos.y);
    if (nbytes > 0) buff[nbytes + 1] = '\0';

    float text_width = MeasureText(buff, TILE_TEXT_SIZE);
    float text_x_offset = text_width * 0.5; // center text horizontally
    float text_y_offset = TILE_TEXT_SIZE * 0.5; // center text vertically
    DrawRectangle(x_offset - text_x_offset - TILE_TEXT_PADDING, y_offset - text_y_offset - TILE_TEXT_PADDING, text_width + TILE_TEXT_PADDING * 2, TILE_TEXT_SIZE + TILE_TEXT_PADDING * 2, Fade(BLACK, 0.5f));
    DrawText(buff, x_offset - text_x_offset, y_offset - text_y_offset, TILE_TEXT_SIZE, WHITE);
#undef TILE_TEXT_SIZE
#undef TILE_TEXT_PADDING
}
