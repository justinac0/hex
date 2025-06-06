#ifndef UTIL_HPP
#define UTIL_HPP

#include <raylib.h>
#include <raymath.h>
#include <assert.h>

static Color colors[] = {
    LIGHTGRAY,
    GRAY,
    DARKGRAY,
    YELLOW,
    GOLD,
    ORANGE,
    PINK,
    RED,
    MAROON,
    GREEN,
    LIME,
    DARKGREEN,
    SKYBLUE,
    BLUE,
    DARKBLUE,
    PURPLE,
    VIOLET,
    DARKPURPLE,
    BEIGE,
    BROWN,
    DARKBROWN,
    MAGENTA,
};

#define ARRAY_SIZE(array, type) (sizeof(array) / sizeof(type))

#define TEXTBOX_PADDING 10
static void DrawTextBox(const char *text, int x, int y, Color color, int fontSize) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    int boxWidth = textWidth + TEXTBOX_PADDING * 2;
    int boxHeight = textHeight + TEXTBOX_PADDING * 2;
    DrawRectangle(x, y, boxWidth, boxHeight, Fade(BLACK, 0.5f));
    DrawText(text, x + TEXTBOX_PADDING, y + TEXTBOX_PADDING, fontSize, color);
}

static void DrawTextBoxCentered(const char *text, int x, int y, Color color, int fontSize) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    int boxWidth = textWidth + TEXTBOX_PADDING * 2;
    int boxHeight = textHeight + TEXTBOX_PADDING * 2;

    int boxX = x - boxWidth / 2;
    int boxY = y - boxHeight / 2;
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.5f));
    DrawText(text, boxX + TEXTBOX_PADDING, boxY + TEXTBOX_PADDING, fontSize, color);
}

static Color* GetPixelFromImage(Image* image, int x, int y) {
    assert(x >= 0 && x < image->width);
    assert(y >= 0 && y < image->height);
    return (Color*)(image->data) + y * image->width + x;
}

#endif