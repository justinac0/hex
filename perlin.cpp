#include <raylib.h>
#include <raymath.h>
#include <limits.h>

#include "engine/util.hpp"
#include "engine/window.hpp"
#include "engine/camera.hpp"

// https://en.wikipedia.org/wiki/Smoothstep
float SmoothStep(float edge0, float edge1, float x) {
   x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
   return x * x * (3.0f - 2.0f * x);
}

float InterpolateEdges(float x, float a0, float a1) {
    return a0 + SmoothStep(a0, a1, x) * (a1 - a0);
}

float GetRandomValueFloat(float min, float max) {
    int randValue = GetRandomValue(0, INT_MAX);
    float normalized = (float)randValue / (float)INT_MAX;
    return min + normalized * (max - min);
}

Vector2 Vector2Random() {
    return Vector2Normalize({
        GetRandomValueFloat(-1.0f, 1.0f),
        GetRandomValueFloat(-1.0f, 1.0f)
    });
}

#define PERLIN_NOISE_WIDTH (3)
#define PERLIN_NOISE_HEIGHT (3)
typedef struct PerlinNoiseGenerator {
    Vector2 gradients[PERLIN_NOISE_WIDTH][PERLIN_NOISE_HEIGHT];

    PerlinNoiseGenerator() {
        // Initialize any necessary parameters for Perlin noise generation
        Seed();
    }
    ~PerlinNoiseGenerator() {
        // Clean up any resources if necessary
    }

    void Seed() {
        // Initialize the gradient vectors for the Perlin noise
        for (int x = 0; x < PERLIN_NOISE_WIDTH; x++) {
            for (int y = 0; y < PERLIN_NOISE_HEIGHT; y++) {
                gradients[x][y] = Vector2Random();
            }
        }
    }

    #define LINE_COLOR WHITE
    #define ARROW_COLOR RED
    void Draw() {
        // Draw the grid
        for (size_t x = 0; x < PERLIN_NOISE_WIDTH + 1; x++) {
            float xPos = (float)(x) * ((float) (SCREEN_WIDTH) / PERLIN_NOISE_WIDTH);
            DrawLine(xPos, 0, xPos, SCREEN_HEIGHT, LINE_COLOR);
        }

        for (size_t y = 0; y < PERLIN_NOISE_HEIGHT + 1; y++) {
            float yPos = (float)(y) * ((float) (SCREEN_HEIGHT) / PERLIN_NOISE_HEIGHT);
            DrawLine(0, yPos, SCREEN_WIDTH, yPos, LINE_COLOR);
        }

        for (size_t x = 0; x < PERLIN_NOISE_WIDTH; x++) {
            for (size_t y = 0; y < PERLIN_NOISE_HEIGHT; y++) {
                Vector2 gradient = gradients[x][y];
                float xPos = (float)(x) * ((float) (SCREEN_WIDTH) / PERLIN_NOISE_WIDTH);
                float yPos = (float)(y) * ((float) (SCREEN_HEIGHT) / PERLIN_NOISE_HEIGHT);

                Draw
            }
        }
    }

    Vector2 Sample(Vector2 position) {

        
        
        return Vector2Zero(); // Placeholder for actual Perlin noise sampling logic
    }
} PerlinNoiseGenerator;

unsigned char *GenerateWorldMapSample(int width, int height) {

    
    return nullptr;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "perlin-demo");

    Camera2D camera = {};
    camera.zoom = 0.5f;

    Image noise;
    int width = 512;
    int height = 512;
    noise.data = GenerateWorldMapSample(width, height);    
    noise.height = width;
    noise.width = height;
    noise.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
    noise.mipmaps = 1;

    Texture2D texture = LoadTextureFromImage(noise);
    PerlinNoiseGenerator perlin;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        BeginMode2D(camera);
        UpdateCamera(&camera);
        // DrawTexture(texture, 0, 0, WHITE);
        perlin.Draw();
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}