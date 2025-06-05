#include <iostream>
#include <queue>
#include <assert.h>

#include "engine/util.hpp"
#include "engine/tile.hpp"
#include "engine/window.hpp"
#include "engine/camera.hpp"

Color* GetPixelFromImage(Image* image, int x, int y) {
    assert(x >= 0 && x < image->width);
    assert(y >= 0 && y < image->height);
    return (Color*)(image->data) + y * image->width + x;
}

Color TileColorFromSample(Color* sample) {
    unsigned char random_sample = sample->r;
    
    if (random_sample > 191) {
        return WHITE;
    } else if (random_sample > 127) {
        return GREEN;
    } else if (random_sample > 63) {
        return BLUE;
    } else {
        return DARKBLUE;
    }
}

int main() {
    srand(time(NULL));

    #ifdef DEBUG
    SetTraceLogLevel(LOG_DEBUG);
    #else
    SetTraceLogLevel(LOG_ERROR);
    #endif
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hex");

    // setup camera
    Camera2D camera = {};
    camera.zoom = 1.0f;

    // setup grid
    #define GRID_SIZE (150)
    Image noise = GenImagePerlinNoise(GRID_SIZE, GRID_SIZE, rand()%100, rand()%100, 5.0f);
    Tile grid[GRID_SIZE * GRID_SIZE];
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            size_t i = (size_t)y * GRID_SIZE + (size_t)x;
            Color* sample = GetPixelFromImage(&noise, x, y);
            grid[i] = Tile({x, y});
            grid[i].color = TileColorFromSample(sample);
        }
    }
    #undef GRID_SIZE

    // Tile Selection    
    Tile *current = NULL;

    double startClick = 0;

    Texture2D texture = LoadTextureFromImage(noise);

    // draw, update
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        Vector2 mp = GetMousePosition();
        Vector2 world = GetScreenToWorld2D(mp, camera);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            startClick = GetTime();

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

        for (auto &t : grid)
            t.Draw();

        #ifdef DEBUG
        for (auto &t : grid)
            t.DebugDraw();
        #endif

        if (current)
            current->DrawSelection();

        UpdateCamera(&camera);

        EndMode2D();

        #ifdef DEBUG
        DrawTexture(texture, 0, 0, WHITE);
        #endif

        EndDrawing();
    }

    UnloadImage(noise);

    CloseWindow();
    return 0;
}