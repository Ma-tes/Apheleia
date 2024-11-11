#pragma once

#include <SDL2/SDL.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/font_system.h"
#include "global_state.h"

void log_initialization(global_engine_information *global) {
    SDL_Texture *current_texture = create_texture(TEST_TEXTURE_PATH, BMP, global->renderer,
        (color) { 159, 159, 159 });

    add_texture_atlas((texture_atlas_information){
        .texture = current_texture,
        .size = (vector2) { .x = 1024, .y = 1024 },
        .tile_offset_size = (vector2) { .x = 64, .y = 64 }
    });
}

void log_update(global_engine_information *global, float delta_time) {
}

void log_render(global_engine_information *global) {
    current_texture_index = 1;
    SDL_Renderer *renderer = global->renderer;
    char value_buffer[16];

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    draw_text("------=Test=------", (vector2) { 44, 30 }, colors[BLACK], global->font, 16);
    draw_text("------=Test=------", (vector2) { 40, 30 }, colors[WHITE], global->font, 16);

    draw_text("Main engine", (vector2) { 12, 70 }, colors[BLACK], global->font, 16);
    draw_text("Main engine", (vector2) { 10, 70 }, colors[WHITE], global->font, 16);

    draw_log_message("Current time: ", "%lu", (void*)engine.current_performance_counter,
        (vector2) { 16, 90 }, colors[WHITE], global->font, 8);
    draw_log_message("Event type: ", "%u", engine.global_information->event == NULL ? 0 : (void*)engine.global_information->event->type,
        (vector2) { 16, 110 }, colors[WHITE], global->font, 8);

    draw_text("Logging engine", (vector2) { 12, 170 }, colors[BLACK], global->font, 16);
    draw_text("Logging engine", (vector2) { 10, 170 }, colors[WHITE], global->font, 16);

    draw_log_message("Current time: ", "%lu", (void*)logging_engine.current_performance_counter,
        (vector2) { 16, 190 }, colors[WHITE], global->font, 8);

    draw_log_message("Event type: ", "%u", logging_engine.global_information->event == NULL ? 0 : (void*)logging_engine.global_information->event->type,
        (vector2) { 16, 210 }, colors[WHITE], global->font, 8);
}