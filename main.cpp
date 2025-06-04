#include <iostream>
#include <assert.h>
#include "util.hpp"
#include "tile.hpp"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

Vector2 Vector2Scale(Vector2 a, float scalar) {
    return {
        a.x * scalar,
        a.y * scalar,
    };
}

Vector2 Vector2Add(Vector2 a, Vector2 b) {
    return {
        a.x + b.x,
        a.y + b.y,
    };
}

float Clamp(float value, float min, float max) {
    const float t = value < min ? min : value;
    return t > max ? max : t;
}

void UpdateCamera(Camera2D *camera) {
    assert(camera);

    // move
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
    }

    // scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
        camera->offset = GetMousePosition();
        
        camera->target = mouseWorldPos;
        float scale = 0.2f*wheel;
        camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.125f, 64.0f);
    }
}

int main() {
    srand(time(NULL));

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    // setup camera
    Camera2D camera = {};
    camera.zoom = 1.0f;

    // setup grid
    Tile grid[10000];
    int i = 0;
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            grid[i++] = Tile({x, y});
        }
    }
   
    // draw, update
    while (!WindowShouldClose()) {
        UpdateCamera(&camera);

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

        if (IsKeyPressed(KEY_R)) {
            camera = {};
            camera.zoom = 1.0f;
        }

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