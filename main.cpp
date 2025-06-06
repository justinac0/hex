#include <iostream>
#include <queue>
#include <assert.h>

#include "engine/util.hpp"
#include "engine/window.hpp"
#include "engine/camera.hpp"
#include "engine/tile.hpp"
#include "engine/tilemap.hpp"

int main() {
#ifdef DEBUG
    SetTraceLogLevel(LOG_DEBUG);
#else
    SetTraceLogLevel(LOG_ERROR);
#endif

    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    Camera2D camera;
    InitCamera(&camera);

    TileMap grid(128, 64);
    grid.Generate();

    Tile *current = NULL;

    while (!window.ShouldClose()) {
        BeginDrawing();
        ClearBackground({ 45, 45, 45, 255 });
        BeginMode2D(camera);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mp = GetMousePosition();
            Vector2 world = GetScreenToWorld2D(mp, camera);

            current = grid.GetTile(world.x, world.y);
        }

        grid.Draw();

        if (current)
            current->DrawSelection();

        EndMode2D();

        UpdateCamera(&camera);

        // cursor
        Vector2 mp = GetMousePosition();
        Rectangle rect;
        rect.width = 16;
        rect.height = 16;
        rect.x = mp.x - rect.width/2;
        rect.y = mp.y - rect.height/2;
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, RAYWHITE);
        DrawRectangleLinesEx(rect, 2, BLACK);

        EndDrawing();
    }

    return 0;
}