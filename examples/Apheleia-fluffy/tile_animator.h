#pragma once

#include <sys/time.h>
#include "apheleia_fluffy_global.h"
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/math/vector.h"

enum animation_type {
    FIRE_FORGET,
    REPEAT,
    BOUNCE
};

typedef struct animation_atlas {
    tile_information *textures;
    color color;

    int texture_index;
    int textures_count;

    enum animation_type type;

    Uint32 key_frame_speed;
    Uint32 last_draw_time;
} animation_atlas;

typedef void(*animation_function)(animation_atlas *atlas, SDL_Renderer *renderer,
    const vector2 draw_position, const vector2 size, tile_external_information *external);

void run_fire_forget_animation(animation_atlas *atlas, SDL_Renderer *renderer, const vector2 draw_position, const vector2 size, tile_external_information *external) {
    tile_information texture_tile = atlas->textures[atlas->texture_index];

    texture_tile.draw_tile_f(renderer, texture_tile, draw_position, atlas->color, size, external);

    if((atlas->texture_index + 1) != atlas->textures_count) {
        atlas->texture_index++;
    }
}

void run_repeat_animation(animation_atlas *atlas, SDL_Renderer *renderer, const vector2 draw_position, const vector2 size, tile_external_information *external) {
    tile_information texture_tile = atlas->textures[atlas->texture_index];

    texture_tile.draw_tile_f(renderer, texture_tile, draw_position, atlas->color, size, external);

    atlas->texture_index = (atlas->texture_index + 1) % atlas->textures_count;
}

void run_bounce_animation(animation_atlas *atlas, SDL_Renderer *renderer, const vector2 draw_position, const vector2 size, tile_external_information *external) {
}

static animation_function animation_types_f[3] = {
    [FIRE_FORGET] = run_fire_forget_animation,
    [REPEAT] = run_repeat_animation,
    [BOUNCE] = run_bounce_animation,
};

int64_t get_time_milliseconds() {
    struct timeval current_time; gettimeofday(&current_time, NULL);
    return ((int64_t)(current_time.tv_sec) * 1000) / ((current_time.tv_usec) * 1000);
}

void run_animation(animation_atlas *atlas, SDL_Renderer *renderer, const vector2 draw_position, const vector2 size, tile_external_information *external) {
    //Gets relative animation type function
    animation_function current_animation_type_f = animation_types_f[atlas->type];

    //Calculates time difference, for key frames
    Uint32 current_time = SDL_GetTicks();

    Uint32 draw_time_difference = current_time - atlas->last_draw_time;

    //Calls animation type function, if key frames has to draw
    if(draw_time_difference >= atlas->key_frame_speed) {
        current_animation_type_f(atlas, renderer, draw_position, size, external);
        atlas->last_draw_time = current_time;
    }
    else {
        tile_information texture_tile = atlas->textures[atlas->texture_index];
        texture_tile.draw_tile_f(renderer, texture_tile, draw_position, atlas->color, size, external);
    }
}



