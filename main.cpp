#include <iostream>
#include <queue>
#include <assert.h>

#include "engine/util.hpp"
#include "engine/tile.hpp"
#include "engine/window.hpp"
#include "engine/camera.hpp"

int main() {
    srand(time(NULL));
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    // setup camera
    Camera2D camera = {};
    camera.zoom = 1.0f;

    // setup grid
    #define GRID_SIZE 50
    Tile grid[GRID_SIZE * GRID_SIZE];
    size_t i = 0;
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            grid[i++] = Tile({x, y});
        }
    }
    #undef GRID_SIZE

    // Tile Selection    
    Tile *current = NULL;

    double startClick = 0;

    // draw, update
    while (!WindowShouldClose()) {
        UpdateCamera(&camera);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        Vector2 mp = GetMousePosition();
        Vector2 world = GetScreenToWorld2D(mp, camera);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            startClick = GetTime();
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            double now = GetTime();
            double clickTime = now - startClick;
            if (clickTime < 0.25) {
                for (auto &t : grid) {
                    if (t.ContainsPoint(world)) {
                        current = &t;
                        goto CRIMES;
                    }
                }

                current = NULL;    
            }
        }
        CRIMES: // yeah I did that - mitch

        for (auto &t : grid) {
            t.Draw();
        #ifdef DEBUG
            t.DebugDraw();
        #endif
        }

        if (current) {
            current->DrawSelection();
        }

        if (IsKeyPressed(KEY_R)) {
            camera = {};
            camera.zoom = 1.0f;
        }

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}