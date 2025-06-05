#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <limits.h>

#include "engine/util.hpp"
#include "engine/window.hpp"
#include "engine/camera.hpp"

typedef struct PerlinNoise {
    Image image;
    PerlinNoise(int width, int height, float scale) {
        assert(width > 0 && height > 0);

        image = GenImagePerlinNoise(width, height, 0, 0, scale);
        if (image.data == NULL) {
            TraceLog(LOG_ERROR, "Failed to generate Perlin noise image");
            return;
        }
        TraceLog(LOG_INFO, "Generated Perlin noise image of size %dx%d", width, height);

        for (int y = 0; y < image.height; y++) {
            for (int x = 0; x < image.width; x++) {
                Color* pixel = GetPixel(x, y);

                if (pixel->r > 180) {
                    *pixel = DARKGRAY;
                } else if (pixel->r > 128) {
                    *pixel = DARKGREEN;
                } else if (pixel->r > 115) {
                    *pixel = BLUE;
                } else {
                    *pixel = DARKBLUE;
                }
            }
        }
    }

    Color* GetPixel(int x, int y) {
        assert(x >= 0 && y >= 0);
        assert(x < image.width && y < image.height);

        Color* data = (Color*)image.data;
        return &data[y * image.width + x];
    }

    ~PerlinNoise() {
        UnloadImage(image);
    }

    // float Sample(int x, int y) const {
    //     assert(x > 0 && y > 0);
    //     assert(x < image.width && y < image.height);

    //     float* data = (float*)image.data;
    //     float pixel = data[y * image.width + x];
       
    //     return pixel;        
    // }
} PerlinNoise;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "perlin-demo");

    Camera2D camera = {};
    camera.zoom = 1.0f;

    PerlinNoise perlinNoise(512, 512, 5.0f);
    Texture2D texture = LoadTextureFromImage(perlinNoise.image);
    // Image noise = GenImagePerlinNoise(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 5.0f);
    // Texture2D texture = LoadTextureFromImage(noise);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        BeginMode2D(camera);
        DrawTexture(texture, 0, 0, WHITE);
        
        UpdateCamera(&camera);
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}