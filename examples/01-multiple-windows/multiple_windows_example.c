/**
 * The first example of Apheleia engine, the example was written in 12.11.2024.
 * 
 * It shows a basic template of using:
 *  1. Font system
 *  2. Texture tiles
 *  3. Multiple rendering engine units, with individual windows
 * 
 * Current file represent main starting functionality, moreover
 * the second rendering unit is represented in file: `log_window.h`.
 * 
 * Global states and engines declarations are in file: `global_state.h`.
**/

/**
 * IMPORTANT ENGINE DEFINITIONS, WHICH MUST BE DEFINIED.
 *  - If not, the values will be set automaticaly to default values.
 * 
 * Current represented values of engine declarations are those default ones.
 * 
 * 1. #define TILE_COUNT 1
 * 2. #define RENDERING_FLAGS SDL_RENDERER_ACCELERATED
 * 3. #define SDL_CLEAR_COLOR colors[BLACK]
 * 4. #define MAX_EVENT_HANDLER 1
 * 5. #define MAX_COMPONENT_HANDLER 1
 * 6. #define SDL_INIT_FLAGS SDL_INIT_VIDEO
 * 7. #define MAX_TEXTURES 4
**/
#define TILE_COUNT 4

#include <SDL2/SDL.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/font_system.h"
#include "log_window.h"
#include "global_state.h"

/**
 * Character representation of font atlas in file: "bin/main_texture_sheet.bmp".
**/
#define FONT_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]{}?!:;%%"

/**
 * Color representation of font atlas key color, for creating transparent tiles.
**/
#define TEXTURE_ATLAS_KEY_COLOR (color) { 159, 159, 159 }

vector2 sprites_position = ZERO_VECTOR2;
enum { FIRST, SECOND, THIRD, FOURTH, ERROR};

tile_information tile_informations[TILE_COUNT] = {
    [FIRST] = {
        .name = "First tile",
        .identificator = FIRST,
        .index_position = { .x = 0, .y = 6 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile,
    },
    [SECOND] = {
        .name = "Second tile",
        .identificator = SECOND,
        .index_position = { .x = 1, .y = 6 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile,
    },
    [THIRD] = {
        .name = "Third tile",
        .identificator = THIRD,
        .index_position = { .x = 0, .y = 7 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile,
    },
    [FOURTH] = {
        .name = "Fourth tile",
        .identificator = THIRD,
        .index_position = { .x = 1, .y = 7 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile,
    },
};

void initialization(global_engine_information *global) {
    SDL_Texture *current_texture = create_texture(TEST_TEXTURE_PATH, BMP, global->renderer,
        TEXTURE_ATLAS_KEY_COLOR);

    add_texture_atlas((texture_atlas_information){
        .texture = current_texture,
        .size = (vector2) { .x = 1024, .y = 1024 },
        .tile_offset_size = (vector2) { .x = 64, .y = 64 }
    });
}

void update(global_engine_information *global, float delta_time) {
    sprites_position.y += 0.25 * delta_time;
}

void render(global_engine_information *global) {
    current_texture_index = 0;
    SDL_Renderer *renderer = global->renderer;

    draw_text("TODO", (vector2) { 62, 28 }, colors[WHITE], global->font, 24);
    draw_text("TODO", (vector2) { 60, 30 }, colors[BLUE], global->font, 24);

    char *text = "[0] - Create better folder structure.\n[1] - Create working collision system.\n[2] - Make refactor of engine core.";
    draw_text(text, (vector2) { 60, 60 }, colors[WHITE], global->font, 12);

    draw_text("int x = 0;\nint y = 1;\nif(x < y)\n{\n   x = 1;\n}", (vector2) { 60, 120 }, colors[WHITE], global->font, 12);

    for (int i = 0; i < TILE_COUNT; i++)
    {
        int relative_x_position = 150 * i;
        tile_informations[i].draw_tile_f(renderer, tile_informations[i],
            (vector2) { relative_x_position, sprites_position.y }, colors[WHITE], (vector2) { 50, 50 });

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, relative_x_position, sprites_position.y,
            global->input_handler->cursor.position.x, global->input_handler->cursor.position.y);
    }
}

int main() {
    engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    logging_engine = (engine_state){
        .initialization_f = log_initialization,
        .update_f = log_update,
        .render_f = log_render,
    };
    engine_default_global = create_initialization_global(
        SDL_CreateWindow( "Test window", WINDOW_POSITION_X, WINDOW_POSITION_y,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL
        ), WINDOW_WIDTH, WINDOW_HEIGHT
    );

    logging_engine_default_global = create_initialization_global(
        SDL_CreateWindow( "Logging window", LOGGING_WINDOW_POSITION_X, LOGGING_WINDOW_POSITION_y,
            LOGGING_WINDOW_WIDTH, LOGGING_WINDOW_HEIGHT, SDL_WINDOW_OPENGL
        ), LOGGING_WINDOW_WIDTH, LOGGING_WINDOW_HEIGHT
    );

    engine_default_global.framerate_limit = 120;
    logging_engine_default_global.framerate_limit = 120;

    engine.global_information = &engine_default_global;
    logging_engine.global_information = &logging_engine_default_global;

    engine_default_global.font = (font_information*)malloc(sizeof(font_information));
    initializte_global_font(engine_default_global.font,
        (vector2) {
            .x = 0,
            .y = 0
        }, FONT_SYMBOLS, 16);

    logging_engine_default_global.font = (font_information*)malloc(sizeof(font_information));
    initializte_global_font(logging_engine_default_global.font,
        (vector2) {
            .x = 0,
            .y = 0
        }, FONT_SYMBOLS, 16);

    engine_state *engines[2] = { &engine, &logging_engine };
    start_engines(engines, 2);
}