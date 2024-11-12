#pragma once

#include <math.h>
#include "engine_global.h"
#include "../input.h"

/**
 * Event handler for input events, including `SDL_MOUSEMOTION`
 * and all key events(Represented by enum `input_key_state`).
 * 
 * Event result is saved in global: `struct input *input_handler`.
* */
void input_event(struct global_engine_information *global_information) {
    switch (global_information->event->type)
    {
        //Sets mouse position to `input_handler`, even if it's
        //out of bounds(Handled by SDL library).
        case SDL_MOUSEMOTION: {
            float relative_x = global_information->event->motion.x;
            float relative_y = global_information->event->motion.y;

            global_information->input_handler->cursor.position.x = relative_x;
            global_information->input_handler->cursor.position.y = relative_y;
        }
        //Sets key states to keyboard or mouse input. If current key code is
        //greated then `SDL_NUM_SCANCODES` it's mouse input otherwise it's
        //keyboard input.
        case SDL_KEYDOWN: case SDL_KEYUP: case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            SDL_Scancode key_code = global_information->event->key.keysym.scancode;
            SDL_Event event = *global_information->event;
            if(key_code > SDL_NUM_SCANCODES) {
                /**
                 * Gets related mouse key by current event, which is
                 * based on predefiend SDL values.
                 * 
                 * SDL_BUTTON_LEFT: L_BUTTON;
                 * SDL_BUTTON_RIGHT: R_BUTTON;
                 * SDL_BUTTON_MIDDLE: M_BUTTON;
                **/
                int mouse_key_state = get_mouse_key_state(event);

                global_information->input_handler->buttons[mouse_key_state].key_state =
                    event.type == SDL_MOUSEBUTTONDOWN ? KEY_PRESSED : KEY_UP;
            }
            else {
                global_information->input_handler->buttons[key_code].key_state =
                    event.type == SDL_KEYDOWN ?  KEY_PRESSED : KEY_UP;
            }
        }
    }
}