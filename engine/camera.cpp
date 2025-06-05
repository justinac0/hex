#include "camera.hpp"

#include <math.h>
#include <assert.h>
#include "util.hpp"

void UpdateCamera(Camera2D *camera) {
    assert(camera);

    Vector2 mp = GetMousePosition();
    Vector2 world = GetScreenToWorld2D(mp, *camera);
    DrawCircleV(world, 20.0f, MAGENTA);

    if (IsKeyPressed(KEY_R)) {
        *camera = {};
        camera->zoom = 1.0f;
    }

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
        camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.1f, 1.0f);
    }
}