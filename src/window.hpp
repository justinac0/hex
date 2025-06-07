#ifndef WINDOW_HPP
#define WINDOW_HPP

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();

    bool ShouldClose();
};

#endif