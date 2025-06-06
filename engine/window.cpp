#include "window.hpp"

#include <raylib.h>

Window::Window(int width, int height, const char *title) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    HideCursor();
}

Window::~Window() {
    CloseWindow();
}

bool Window::ShouldClose() {
    return WindowShouldClose();
}