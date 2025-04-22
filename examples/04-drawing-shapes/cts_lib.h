#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/math/vector.h"

#define DEFAULT_CURSOR_SIZE 5

struct cts_points_t
{
    uint16_t m_num_points;
    struct
    {
        uint16_t x;
        uint16_t y;
        uint16_t size;
    } m_points[5];
};

int32_t cts_get_ts_points(cts_points_t *t_points, global_engine_information *global)
{
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    if(!keyboard_state[SDL_SCANCODE_SPACE])
    {
        return 0;
    }

    vector2 cursor_position = global->input_handler->cursor.position;

    t_points->m_num_points = 1;
    t_points->m_points[0].x = cursor_position.x;
    t_points->m_points[0].y = cursor_position.y;
    t_points->m_points[0].size = DEFAULT_CURSOR_SIZE;
}