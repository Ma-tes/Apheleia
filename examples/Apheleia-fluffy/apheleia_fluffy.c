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
#define SDL_CLEAR_COLOR colors[BLACK]
#define SCENE_COUNT 5

#define TILE_DRAWING_OFFSET_Y 60

//#define POST_PIXEL_PROCESS
//#define POST_PIXEL_PROCESSES_COUNT 1

#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/font_system.h"
#include "apheleia_fluffy_global.h"
#include "entity.h"
#include "world_light.h"
#include "world_light_actions.h"
#include "tile_animator.h"
#include "weapon_container.h"
#include "map_editor/map.h"
#include "utilities/tile_helper.h"
#include "utilities/collision.h"
#include "scene.h"
#include "scenes/game_scene.h"
#include "scenes/menu_scene.h"
#include "scenes/settings_scene.h"
#include "scenes/credits_scene.h"
#include "scenes/configuration_scene.h"

/**
 * Character representation of font atlas in file: "bin/main_texture_sheet.bmp".
**/
#define FONT_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]{}?!:;%%"

#define TEST_TEXTURE_PATH "../../bin/test_sprites_3.0.bmp"

/**
 * Color representation of font atlas key color, for creating transparent tiles.
**/
#define TEXTURE_ATLAS_KEY_COLOR (color) { 159, 159, 159 }

/**
 * Fixed width of window, used in global initialization.
**/
#define WINDOW_WIDTH 1024

/**
 * Fixed height of window, used in global initialization.
**/
#define WINDOW_HEIGHT 1024

/**
 * Fixed position x of window, used in global initialization.
**/
#define WINDOW_POSITION_X 400 

/**
 * Fixed position y of window, used in global initialization.
**/
#define WINDOW_POSITION_Y 0

#define PI 3.14159265359

#define INDEXED_ANIMATION_TILE(x, y)        \
    (tile_information) {                    \
        .index_position = { x, y },         \
        .type = BLOCK_TILE,                 \
        .draw_tile_f = draw_tile            \
    }                                       \

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

scene engine_scenes[SCENE_COUNT] = {
    (scene) {
        .identificator_name = "MENU SCENE",
        .identificator = 0,
        .execute_initialization = true,

        .initialization = initialization_menu_scene,
        .update = update_menu_scene,
        .render = render_menu_scene
    },
    (scene) {
        .identificator_name = "GAME SCENE",
        .identificator = 1,
        .execute_initialization = true,

        .initialization = initialization_game_scene,
        .update = update_game_scene,
        .render = render_game_scene
    },
    (scene) {
        .identificator_name = "SETTINGS SCENE",
        .identificator = 2,
        .execute_initialization = true,

        .initialization = initialization_settings_scene,
        .update = update_settings_scene,
        .render = render_settings_scene
    },
    (scene) {
        .identificator_name = "CREDITS SCENE",
        .identificator = 3,
        .execute_initialization = true,

        .initialization = initialization_credits_scene,
        .update = update_credits_scene,
        .render = render_credits_scene
    },
    (scene) {
        .identificator_name = "CONFIGURATION SCENE",
        .identificator = 4,
        .execute_initialization = true,

        .initialization = initialization_configuration_scene,
        .update = update_configuration_scene,
        .render = render_configuration_scene
    }
};

void initialization(global_engine_information *global) {
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
        .size = (vector2) { .x = 2048, .y = 2048 },
        .tile_offset_size = (vector2) { .x = 64, .y = 64 }
    });

    execute_scene_initialization(&engine_scenes[current_scene_identificator], global);
}

void update(global_engine_information *global, float delta_time) {
    execute_scene_update(&engine_scenes[current_scene_identificator], global, delta_time);
}

void render(global_engine_information *global) {
    execute_scene_render(&engine_scenes[current_scene_identificator], global);
}

int main() {
    engine_state engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    global_engine_information engine_default_global = create_initialization_global(
        SDL_CreateWindow("Test window", WINDOW_POSITION_X, WINDOW_POSITION_Y,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        ), WINDOW_WIDTH, WINDOW_HEIGHT
    );

    engine_default_global.framerate_limit = 120;
    engine.global_information = &engine_default_global;

    engine_default_global.font = (font_information*)malloc(sizeof(font_information));
    initializte_global_font(engine_default_global.font,
        (vector2) {
            .x = 0,
            .y = 11
        }, FONT_SYMBOLS, 16);

    apheleia_state = &engine;
    start_engine(&engine);
}