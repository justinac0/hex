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

#define TILE_TEXT_SIZE 48
void Tile::DebugDraw() {
    float tessellation_width = TILE_RADIUS + TILE_RADIUS * 0.5; // r + r*cos(pi/3); the axis-aligned width + the slanted side width
    float tessellation_height = TILE_RADIUS * sinf(PI/3) * 2; // r*sin(pi/3)*2; the height slanted side height times 2;
    bool is_odd_column = (this->pos.x % 2);
    float x_offset = this->pos.x * tessellation_width;
    float y_offset = this->pos.y * tessellation_height + (is_odd_column * tessellation_height * 0.5);
    
    char buff[256];
    int status = sprintf(buff, "%d, %d", pos.x, pos.y);
    if (status > 0) buff[status + 1] = '\0';
    int length = TextLength(buff);

    float text_x_offset = TILE_TEXT_SIZE * 0.5 * length; // center text horizontally
    float text_y_offset = -TILE_TEXT_SIZE * 0.5; // center text vertically
    DrawText(buff, x_offset - (length * , y_offset - TILE_TEXT_SIZE / 2, TILE_TEXT_SIZE, WHITE);
}
#undef TILE_TEXT_SIZE