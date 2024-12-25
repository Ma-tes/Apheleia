#pragma once

#include <stdint.h>

#define COLORS_COUNT 10 

enum color_types {
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    YELLOW,
    BROWN,
    GRAY,
    DARK_GRAY
};

typedef struct color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

static color colors[COLORS_COUNT] = {
    [WHITE] = (color) { 255, 255, 255 },
    [BLACK] = (color) { 0, 0, 0 },
    [RED] = (color) { 255, 0, 0 },
    [GREEN] = (color) { 0, 255, 0 },
    [BLUE] = (color) { 0, 0, 255 },
    [ORANGE] = (color) { 255, 165, 0 },
    [YELLOW] = (color) { 255, 255, 0 },
    [BROWN] = (color) { 165, 42, 42 },
    [GRAY] = (color) { 105, 105, 105 },
    [DARK_GRAY] = (color) { 32, 32, 32 },
};