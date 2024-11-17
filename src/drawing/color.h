#pragma once

#include <stdint.h>

#define COLORS_COUNT 9 

enum color_types {
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    BROWN,
    GRAY,
    DARK_GRAY
};

typedef struct color {
    int8_t r;
    int8_t g;
    int8_t b;
} color;

static color colors[COLORS_COUNT] = {
    [WHITE] = (color) { 255, 255, 255},
    [BLACK] = (color) { 0, 0, 0},
    [RED] = (color) { 255, 0, 0},
    [GREEN] = (color) { 0, 255, 0},
    [BLUE] = (color) { 0, 0, 255},
    [YELLOW] = (color) { 255, 255, 0},
    [BROWN] = (color) { 165, 42, 42},
    [GRAY] = (color) { 105, 105, 105},
    [DARK_GRAY] = (color) { 32, 32, 32},
};