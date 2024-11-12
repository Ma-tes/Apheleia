#pragma once

#include <SDL2/SDL.h>
#include "math/vector.h"

#define CURSOR_NUMBER_SCANCODES 3

enum input_key_state {
    KEY_UP, 
    KEY_PRESSED,
};

enum mouse_key_state {
    L_BUTTON = SDL_BUTTON_LEFT + 0,
    M_BUTTON = SDL_BUTTON_MIDDLE + 1,
    R_BUTTON = SDL_BUTTON_RIGHT + 2,
    NONE = SDL_SCANCODE_END //TODO: Use valid null value.
};

struct cursor_input {
    vector2 position;
    vector2 tile_position;
};

typedef struct input {
    struct cursor_input cursor;

    struct {
        enum input_key_state key_state;
        uint64_t key_time;
    } buttons[SDL_NUM_SCANCODES + CURSOR_NUMBER_SCANCODES];

} input;

void create_input(input **input_a) {
    *input_a = malloc(sizeof(input));
    (*input_a)->cursor = (struct cursor_input) {
            .position = ZERO_VECTOR2,
            .tile_position = ZERO_VECTOR2
    };
}

static enum mouse_key_state get_mouse_key_state(const SDL_Event event) {
    switch (event.button.button)
    {
        case SDL_BUTTON_LEFT: return L_BUTTON;
        case SDL_BUTTON_RIGHT: return R_BUTTON;
        case SDL_BUTTON_MIDDLE: return M_BUTTON;
    }
    return NONE;
};