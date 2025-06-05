#include <assert.h>
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

typedef struct Vector2i {
    size_t x;
    size_t y;
} Vector2i;

#define PERLIN_NOISE_WIDTH (8)
#define PERLIN_NOISE_HEIGHT (8)
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

    void DrawDebug() {
        for (int x = 0; x < PERLIN_NOISE_WIDTH; x++) {
            for (int y = 0; y < PERLIN_NOISE_HEIGHT; y++) {
                float xPos = (float)(x) * ((float) (SCREEN_WIDTH) / (PERLIN_NOISE_WIDTH - 1));
                float yPos = (float)(y) * ((float) (SCREEN_HEIGHT) / (PERLIN_NOISE_HEIGHT - 1));

                float sample = Sample({xPos, yPos});

                DrawCircleV(
                    {xPos, yPos},
                    10.0f,
                    ColorAlpha(PURPLE, sample)
                );
                DrawText(
                    TextFormat("%.5f", sample),
                    (int)xPos + 10,
                    (int)yPos + 10,
                    20,
                    GREEN
                );
            }
        }
    }

    #define LINE_COLOR WHITE
    #define LINE_WIDTH (4.0f)
    #define GRADIENT_ARROW_COLOR RED
    #define ARROW_LENGTH (100.0f)
    #define OFFSET_ARROW_COLOR YELLOW
    void Draw(Camera2D camera) {
        // Draw the grid
        for (size_t x = 0; x < PERLIN_NOISE_WIDTH - 1; x++) {
            float xPos = (float)(x) * ((float) (SCREEN_WIDTH) / (PERLIN_NOISE_WIDTH - 1));
            DrawLineEx({xPos, 0}, {xPos, SCREEN_HEIGHT}, LINE_WIDTH, LINE_COLOR);
        }
        DrawLineEx({SCREEN_WIDTH, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, LINE_WIDTH, LINE_COLOR);

        for (size_t y = 0; y < PERLIN_NOISE_HEIGHT - 1; y++) {
            float yPos = (float)(y) * ((float) (SCREEN_HEIGHT) / (PERLIN_NOISE_HEIGHT - 1));
            DrawLineEx({0, yPos}, {SCREEN_WIDTH, yPos}, LINE_WIDTH, LINE_COLOR);
        }
        DrawLineEx({0, SCREEN_HEIGHT}, {SCREEN_WIDTH, SCREEN_HEIGHT}, LINE_WIDTH, LINE_COLOR);

        for (size_t x = 0; x < PERLIN_NOISE_WIDTH; x++) {
            for (size_t y = 0; y < PERLIN_NOISE_HEIGHT; y++) {
                Vector2 gradient = gradients[x][y];
                float xPos = (float)(x) * ((float) (SCREEN_WIDTH) / (PERLIN_NOISE_WIDTH - 1));
                float yPos = (float)(y) * ((float) (SCREEN_HEIGHT) / (PERLIN_NOISE_HEIGHT - 1));

                DrawLineEx(
                    {xPos, yPos},
                    {xPos + gradient.x * ARROW_LENGTH, yPos + gradient.y * ARROW_LENGTH},
                    10.0f,
                    GRADIENT_ARROW_COLOR
                );
            }
        }

        Vector2 mp = GetMousePosition();
        Vector2 world = GetScreenToWorld2D(mp, camera);

        Vector2 samplePosition = world;
        Vector2i cell = GetCell(samplePosition);

        if (cell.x >= PERLIN_NOISE_WIDTH - 1 || cell.y >= PERLIN_NOISE_HEIGHT - 1) return;

        
        float interp = Interpolate(
            GetOffsetDot({cell.x, cell.y}, samplePosition), // Top Left
            GetOffsetDot({cell.x + 1, cell.y}, samplePosition), // Top Right
            GetOffsetDot({cell.x, cell.y + 1}, samplePosition), // Bottom Left
            GetOffsetDot({cell.x + 1, cell.y + 1}, samplePosition), // Bottom Right
            (samplePosition.x - GetCellWorld(cell).x) / ((float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1)),
            (samplePosition.y - GetCellWorld(cell).y) / ((float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1))
        );
        DrawRectangle(
            GetCellWorld(cell).x,
            GetCellWorld(cell).y,
            (float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1),
            (float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1),
            ColorAlpha(GREEN, interp * 0.5f + 0.5f) // Interpolated color based on the sample value
        );

        DrawOffset({cell.x, cell.y}, samplePosition); // Top Left
        DrawOffset({cell.x + 1, cell.y}, samplePosition); // Top Right
        DrawOffset({cell.x, cell.y + 1}, samplePosition); // Bottom Left
        DrawOffset({cell.x + 1, cell.y + 1}, samplePosition); // Bottom Right
    }

    void DrawOffset(Vector2i cell, Vector2 position) {
        Vector2 offset = GetOffset(cell, position);
        float dot = Vector2DotProduct(gradients[cell.x][cell.y], offset);
        Vector2 worldPosition = GetCellWorld(cell);
        DrawLineEx(
            worldPosition,
            Vector2Add(worldPosition, offset),
            abs(dot) / 10.0f,
            OFFSET_ARROW_COLOR
        );
    }

    float GetOffsetDot(Vector2i cell, Vector2 position) {
        Vector2 offset = GetOffset(cell, position);
        float dot = Vector2DotProduct(gradients[cell.x][cell.y], offset);
        return dot;
    }

    float Fade(float t) {
        return ((6.0f * t - 15.0f) * t + 10.0f) * t * t * t;
    }
    
    // x and y are in the range [0, 1]
    float Interpolate(float tl, float tr, float bl, float br, float x, float y) {
        float fadeX = Fade(x);
        float fadeY = Fade(y);
        float topX = Lerp(tl, tr, fadeX);
        float bottomX = Lerp(bl, br, fadeX);
        return Lerp(topX, bottomX, fadeY);

        // float leftY = SmoothStep(tl, bl, y);
        // float rightY = SmoothStep(tr, br, y);
        // return SmoothStep(leftY, rightY, x);

        // Vector2 pos = {x, y};
        // Vector2 tlPos = {0, 0};
        // Vector2 trPos = {1, 0};
        // Vector2 blPos = {0, 1};
        // Vector2 brPos = {1, 1};

        // float total_dist = 0;
        // total_dist += Vector2Distance(tlPos, pos);
        // total_dist += Vector2Distance(trPos, pos);
        // total_dist += Vector2Distance(blPos, pos);
        // total_dist += Vector2Distance(brPos, pos);
        
        // float sum = 0;
        // sum += tl * Vector2Distance(tlPos, pos) / total_dist;
        // sum += tr * Vector2Distance(trPos, pos) / total_dist;
        // sum += bl * Vector2Distance(blPos, pos) / total_dist;
        // sum += br * Vector2Distance(brPos, pos) / total_dist;

        // return sum;
    }

    Vector2i GetCell(Vector2 position) {
        if (position.x < 0 || position.x > SCREEN_WIDTH ||
            position.y < 0 || position.y > SCREEN_HEIGHT) {
            return {SSIZE_MAX, SSIZE_MAX};
        }
        
        return {
            (size_t)(position.x / ((float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1))),
            (size_t)(position.y / ((float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1))),
        };
    }

    Vector2 GetCellWorld(Vector2i cell) {
        float cellWidth = (float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1);
        float cellHeight = (float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1);

        return {
            cell.x * cellWidth,
            cell.y * cellHeight
        };
    }

    Vector2 GetOffset(Vector2i cell, Vector2 position) {
        float cellWidth = (float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1);
        float cellHeight = (float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1);

        return {
            position.x - (cell.x * cellWidth),
            position.y - (cell.y * cellHeight)
        };
    }
    
    float Sample(Vector2 position) {
        SetTraceLogLevel(LOG_DEBUG);
        Vector2i cell = GetCell(position);
        if (cell.x >= PERLIN_NOISE_WIDTH - 1 || cell.y >= PERLIN_NOISE_HEIGHT - 1) {
            TraceLog(LOG_ERROR, "Sample position out of bounds: (%f, %f) cell [%lu, %lu]", position.x, position.y, cell.x, cell.y);
            return 0; // Out of bounds
        }

        float tl = GetOffsetDot({cell.x, cell.y}, position); // Top Left
        float tr = GetOffsetDot({cell.x + 1, cell.y}, position); // Top Right
        float bl = GetOffsetDot({cell.x, cell.y + 1}, position); // Bottom Left
        float br = GetOffsetDot({cell.x + 1, cell.y + 1}, position); // Bottom Right

        float x = (position.x - GetCellWorld(cell).x) / ((float)SCREEN_WIDTH / (PERLIN_NOISE_WIDTH - 1));
        float y = (position.y - GetCellWorld(cell).y) / ((float)SCREEN_HEIGHT / (PERLIN_NOISE_HEIGHT - 1));

        return Interpolate(tl, tr, bl, br, x, y);
    }
} PerlinNoiseGenerator;

unsigned char *GenerateWorldMapSample(PerlinNoiseGenerator perlin, int width, int height) {
    unsigned char *data = (unsigned char *)calloc(width*height, sizeof(unsigned char));
    assert(data);

    // work out why the edge is not good
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            float sample = perlin.Sample({(float)x, (float)y});
            // Scale the sample to the range [0, 255]
            unsigned char value = (unsigned char)(Clamp(sample * 255.0f, 0.0f, 255.0f));
            data[y * width + x] = value;
        }
    }

    return data;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "perlin-demo");

    Camera2D camera = {};
    camera.zoom = 1.0f;

    PerlinNoiseGenerator perlin;

    Image noise;
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    noise.data = GenerateWorldMapSample(perlin, width, height);    
    noise.width = width;
    noise.height = height;
    noise.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
    noise.mipmaps = 1;

    Texture2D texture = LoadTextureFromImage(noise);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        BeginMode2D(camera);
        UpdateCamera(&camera);
        DrawTexture(texture, 0, 0, WHITE);
        // perlin.Draw(camera);
        perlin.DrawDebug();

        EndMode2D();
        Vector2 mp = GetMousePosition();
        DrawCircleV(mp, 10.0f, MAGENTA);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}