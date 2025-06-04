#include <iostream>

#include "tile.hpp"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

int main() {
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    // setup camera
    Camera2D camera;
    camera.offset = {SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
    camera.target = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // setup grid
    Tile grid[100];
    int i = 0;
    for (int y = -5; y < 5; y++) {
        for (int x = -5; x < 5; x++) {
            grid[i++] = Tile({x, y});
        }
    }

    // draw, update
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        for (auto &t : grid) {
            t.Draw();
        }

        #ifdef DEBUG
        for (auto t : grid) {
            t.DebugDraw();
        }
        #endif

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}