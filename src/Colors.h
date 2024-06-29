#ifndef COLORS_H
#define COLORS_H

#include "raylib.h"

// array of colors
static Color colors[] = {
    RED, BLUE, GREEN, (Color){254, 213, 0, 255}, ORANGE, PURPLE,
};

#define NUM_COLORS (sizeof(colors) / sizeof(Color))

#endif