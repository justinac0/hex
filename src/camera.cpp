#include "camera.hpp"

#include <iostream>
#include <math.h>
#include <assert.h>
#include "util.hpp"

void InitCamera(Camera2D *camera) {
    assert(camera);

    *camera = {};
    camera->zoom = 1.0f;
}

void UpdateCamera(Camera2D *camera, Vector2 *poffset) {
    assert(camera);
    assert(poffset);

    Vector2 mp = GetMousePosition();
    Vector2 world = GetScreenToWorld2D(mp, *camera);

    if (IsKeyPressed(KEY_R)) {
        *camera = {};
        camera->zoom = 1.0f;
    }

    // move
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
        *poffset = Vector2Add(camera->target, delta);
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