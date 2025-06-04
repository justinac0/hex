#include <iostream>

#include "util.hpp"
#include "tile.hpp"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

int main() {
    srand(time(NULL));

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    // setup camera
    Camera2D camera;
    // camera.offset = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    camera.target = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // setup grid
    Tile grid[100];
    int i = 0;
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            grid[i++] = Tile({x, y});
        }
    }
   
    // draw, update
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        Vector2 mp = GetMousePosition();
        Vector2 world = GetScreenToWorld2D(mp, camera);
        
        for (auto &t : grid) {
            t.Draw();
        }
        
        #ifdef DEBUG
        for (auto t : grid) {
            t.DebugDraw();
        }
        #endif

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || true) {
            for (auto &t : grid) {
                if (t.ContainsPoint(world)) {
                    t.DrawSelection();
                }
            }
        }
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}