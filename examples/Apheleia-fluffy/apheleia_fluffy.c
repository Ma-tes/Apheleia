/*
 * The project example of Apheleia engine, which was in develop since 14.11.2024.
 * 
 * It shows a basic template of using:
 *  1. Font system
 *  2. Texture tiles
 *  3. Input handling for changing sprite position
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
#define TILE_COUNT 1

#include <SDL2/SDL.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/font_system.h"

/**
 * Character representation of font atlas in file: "bin/main_texture_sheet.bmp".
**/
#define FONT_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]{}?!:;%%"

#define TEST_TEXTURE_PATH "../../bin/main_texture_sheet.bmp"

/**
 * Color representation of font atlas key color, for creating transparent tiles.
**/
#define TEXTURE_ATLAS_KEY_COLOR (color) { 159, 159, 159 }

/**
 * Fixed width of window, used in global initialization.
**/
#define WINDOW_WIDTH 800

/**
 * Fixed height of window, used in global initialization.
**/
#define WINDOW_HEIGHT 600

/**
 * Fixed position x of window, used in global initialization.
**/
#define WINDOW_POSITION_X 400 

/**
 * Fixed position y of window, used in global initialization.
**/
#define WINDOW_POSITION_y 0

/**
 * Global testing vector for one tile.
**/
vector2 *sprite_position;

/**
 * Global extern initialization of manualy created tiles.
**/
tile_information tile_informations[TILE_COUNT] = {
    (tile_information) {
        .name = "First tile",
        .identificator = 0,
        .index_position = { .x = 0, .y = 6 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile,
    },
};

void initialization(global_engine_information *global) {
    sprite_position = malloc(sizeof(vector2));
    sprite_position->x = ZERO_VECTOR2.x;
    sprite_position->y = ZERO_VECTOR2.y;

    /**
     * Firstly we create texture, where we have to specify
     * it's own path, file format and tranparent key color.
    **/
    SDL_Texture *current_texture = create_texture(TEST_TEXTURE_PATH, BMP, global->renderer,
        TEXTURE_ATLAS_KEY_COLOR);

    /**
     * Secondly we define tile mapper, where we have to
     * set a size of related texture and size of one tile.
    **/
    add_texture_atlas((texture_atlas_information){
        .texture = current_texture,
        .size = (vector2) { .x = 1024, .y = 1024 },
        .tile_offset_size = (vector2) { .x = 64, .y = 64 }
    });
}

void update(global_engine_information *global, float delta_time) {
    /**
     * Use `SDL_GetKeyboardState` instead of `input_handler` for
     * any precise movement implementations.
    **/
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    if(keyboard_state[SDL_SCANCODE_W]) {
        sprite_position->y -= 1.25 * delta_time;
    }
    if(keyboard_state[SDL_SCANCODE_S]) {
        sprite_position->y += 1.25 * delta_time;
    }
    if(keyboard_state[SDL_SCANCODE_A]) {
        sprite_position->x -= 1.25 * delta_time;
    }
    if(keyboard_state[SDL_SCANCODE_D]) {
        sprite_position->x += 1.25 * delta_time;
    }
}

void render(global_engine_information *global) {
    tile_informations[0].draw_tile_f(global->renderer, tile_informations[0],
        *sprite_position, colors[WHITE], (vector2) { 64, 64 });

    u_long position_x = (u_long)sprite_position->x;
    u_long position_y = (u_long)sprite_position->y;

    vector2 log_message_x_position = sub(*sprite_position, (vector2) { -8, 32 });
    draw_log_message("x: ", "%lu", (void*)position_x,
        log_message_x_position, colors[WHITE], global->font, 8);

    vector2 log_message_y_position = sub(*sprite_position, (vector2) { -8, 16});
    draw_log_message("y: ", "%lu", (void*)position_y,
        log_message_y_position, colors[WHITE], global->font, 8);
}

int main() {
    engine_state engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    global_engine_information engine_default_global = create_initialization_global(
        SDL_CreateWindow( "Test window", WINDOW_POSITION_X, WINDOW_POSITION_y,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL
        ), WINDOW_WIDTH, WINDOW_HEIGHT
    );

    engine_default_global.framerate_limit = 120;
    engine.global_information = &engine_default_global;

    engine_default_global.font = (font_information*)malloc(sizeof(font_information));
    initializte_global_font(engine_default_global.font,
        (vector2) {
            .x = 0,
            .y = 0
        }, FONT_SYMBOLS, 16);

    start_engine(&engine);
}