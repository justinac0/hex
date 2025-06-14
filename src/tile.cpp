#include "tile.hpp"
#include "util.hpp"

Tile::Tile() {
    this->pos = {0,0};
    int n = rand() % ARRAY_SIZE(colors, Color);
    this->color = colors[n];
}

Tile::Tile(Vec2i pos) {
    this->pos = pos;
    int n = rand() % ARRAY_SIZE(colors, Color);
    this->color = colors[n];

    Vector2 position = this->GetWorldPosition();
    for (int i = 0; i < 6; i++) {
        float angle = PI / 3 * i;
        this->points[i] = {position.x + TILE_RADIUS * cosf(angle), position.y + TILE_RADIUS * sinf(angle)};
    }
}

void Tile::Draw() {
    Vector2 position = this->GetWorldPosition();

    DrawPoly(position, 6, TILE_RADIUS, 0, this->color);

#ifdef DEBUG
    if (!discovered) {
        DrawPoly(position, 6, TILE_RADIUS, 0, Fade({75,75,75,255}, 0.97));
    }
#endif
}

void Tile::GetNeighbors(Vec2i dst[6]) {
    if ((pos.x % 2) == 0 && pos.x != 0) {
        dst[0] = {pos.x, pos.y-1};
        dst[1] = {pos.x, pos.y+1};
        dst[2] = {pos.x-1, pos.y};
        dst[3] = {pos.x+1, pos.y};
        dst[4] = {pos.x-1, pos.y-1};
        dst[5] = {pos.x+1, pos.y-1};
        return;
    }

    dst[0] = {pos.x-1, pos.y};
    dst[1] = {pos.x+1, pos.y};
    dst[2] = {pos.x-1, pos.y+1};
    dst[3] = {pos.x+1, pos.y+1};
    dst[4] = {pos.x, pos.y-1};
    dst[5] = {pos.x, pos.y+1};
}

void Tile::DrawSelection() {
    Vector2 position = this->GetWorldPosition();

    int thickness = TILE_RADIUS/6;
    DrawPolyLinesEx(position, 6, TILE_RADIUS+thickness, 0, thickness, MAGENTA);
}

void Tile::DebugDraw() {
    Vector2 position = this->GetWorldPosition();
    
    // tile position to string
    char buff[256];
    int nbytes = sprintf(buff, "%d, %d", pos.x, pos.y);
    if (nbytes > 0) buff[nbytes + 1] = '\0';

    DrawTextBoxCentered(buff, position.x, position.y, WHITE, 24);
}

Vector2 Tile::GetWorldPosition() {
    float tessellation_width = TILE_RADIUS + TILE_RADIUS * 0.5; // r + r*cos(pi/3); the axis-aligned width + the slanted side width
    float tessellation_height = TILE_RADIUS * sinf(PI/3) * 2; // r*sin(pi/3)*2; the height slanted side height times 2;

    bool is_odd_column = (this->pos.x % 2);

    float x_offset = this->pos.x * tessellation_width;
    float y_offset = this->pos.y * tessellation_height + (is_odd_column * tessellation_height * 0.5);

    return {x_offset, y_offset};
}

Vector2* Tile::GetWorldVertices() {
   return this->points;
}

bool Tile::ContainsPoint(Vector2 point) {
    return CheckCollisionPointPoly(point, this->points, 6);
}
