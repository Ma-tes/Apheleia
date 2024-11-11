#pragma once

#include "engine_global.h"
#include <math.h>

void input_event(struct global_engine_information *global_information) {
    switch (global_information->event->type)
    {
        case SDL_MOUSEMOTION: {
            float relative_x = global_information->event->motion.x;
            float relative_y = global_information->event->motion.y;
            if(relative_x != -0 && relative_y != -0) {
                global_information->input_handler->cursor.position.x = relative_x;
                global_information->input_handler->cursor.position.y = relative_y;
            }
        }
        case SDL_KEYDOWN: case SDL_KEYUP: case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            SDL_Scancode key_code = global_information->event->key.keysym.scancode;
            SDL_Event event = *global_information->event;
            if(key_code > SDL_NUM_SCANCODES) {
                int mouse_key_state = get_mouse_key_state(event);

                global_information->input_handler->buttons[mouse_key_state].key_state =
                    event.type == SDL_MOUSEBUTTONDOWN ? KEY_PRESSED : KEY_UP;
            }
            else {
                global_information->input_handler->buttons[key_code].key_state = event.type == SDL_KEYDOWN ?  KEY_PRESSED : KEY_UP;
            }
        }
    }
}