#pragma once

#include <SDL2/SDL.h>
#include "math/vector.h"

//Definition of maximum cursor scancodes.
//TODO: Improve it, by supporting up to four more additional mouse inputs.
#define CURSOR_NUMBER_SCANCODES 3

/**
 * An enum representing key states. It's being used in `input` structure
 * for every specific button.
**/
enum input_key_state {
    KEY_UP, 
    KEY_PRESSED,
};

/**
 * An enum representing mouse key states. It's used for custom remapping
 * of SDL mouse key states, for easier implementation.
**/
enum mouse_key_state {
    L_BUTTON = SDL_BUTTON_LEFT + 0,
    M_BUTTON = SDL_BUTTON_MIDDLE + 1,
    R_BUTTON = SDL_BUTTON_RIGHT + 2,
    NONE = SDL_SCANCODE_END //TODO: Use valid null value.
};

/**
 * A structure representing position of cursor, with it's
 * relative recalculation to `tile_position`.
 * 
 * TODO: Make the `tile_position` calculation, only if there
 * is necessary tile based texture.
**/
struct cursor_input {
    vector2 position;
    vector2 tile_position;
};

/**
 * A structure representing event result from input button
 * states and mouse position(`cursor_input`).
 * 
 * The number of buttons: `SDL_NUM_SCANCODES` + `CURSOR_NUMBER_SCANCODES`.
 * Each key state has it's own key_time of execution.
**/
typedef struct input {
    struct cursor_input cursor;

    struct {
        enum input_key_state key_state;
        bool is_key_pressed;
    } buttons[SDL_NUM_SCANCODES + CURSOR_NUMBER_SCANCODES];

} input;

/**
 * Creates initialization for `input` structure, it's own dynamic
 * allocation. Futhermore `cursor_input` positions are set to `ZERO_VECTOR2`.
**/
void create_input(input **input_a) {
    *input_a = malloc(sizeof(input));
    (*input_a)->cursor = (struct cursor_input) {
            .position = ZERO_VECTOR2,
            .tile_position = ZERO_VECTOR2
    };
}

/**
 * Converts SDL mouse button state to own `mouse_key_state`,
 * for easier implementation.
**/
static enum mouse_key_state get_mouse_key_state(const SDL_Event event) {
    switch (event.button.button)
    {
        case SDL_BUTTON_LEFT: return L_BUTTON;
        case SDL_BUTTON_RIGHT: return R_BUTTON;
        case SDL_BUTTON_MIDDLE: return M_BUTTON;
    }
    return NONE;
}