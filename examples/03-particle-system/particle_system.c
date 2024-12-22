/**
 * The second example of Apheleia engine, which was written in 13.11.2024.
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
#include <time.h>
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

typedef struct entity {
    vector2 position;
    vector2 size;

    tile_information tile;
    color color;
} entity;

typedef struct particle {
    float life_span;
    float velocity;

    entity entity; //Add multiple entities, for any transitions.
} particle;

typedef struct particle_information {
    float life_span;
    float range;

    float origin_velocity;
    entity origin_entity;

    int particle_count;
    particle *particles;
} particle_information;

static particle particle_entities[1024];

static particle test_particle = (particle) {
    .life_span = 20,
    .velocity = 20,
    .entity = (entity) {
        .position = (vector2){ 50, 50 },
        .size = (vector2){ 12, 12 }
    }
};

static particle_information test_particle_event = (particle_information) {
    .life_span = 5,
    .range = 120,

    .origin_velocity = 20,
    .origin_entity = (entity) {
        .position = (vector2){ 50, 50 },
        .size = (vector2){ 12, 12 }
    },
    .particle_count = 100,
};

void initialize_particles(particle_information *information) {
}

void initialization(global_engine_information *global) {
    test_particle.entity.tile = tile_informations[0];
    test_particle.entity.color = colors[WHITE];
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
    test_particle.life_span -= (1.0f / 75);

    test_particle.velocity = SDL_clamp(test_particle.velocity - (0.2f * delta_time), 0, 100);
    vector2 right_velocity_vector = (vector2) { test_particle.velocity, 0 };

    test_particle.entity.position = add(test_particle.entity.position, right_velocity_vector);
}

void render(global_engine_information *global) {
    if((int)(test_particle.life_span) > 0) {
        test_particle.entity.tile.draw_tile_f(global->renderer, test_particle.entity.tile,
            test_particle.entity.position, test_particle.entity.color, test_particle.entity.size);
    }
}

int main() {
    engine_state engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    global_engine_information engine_default_global = create_initialization_global(
        SDL_CreateWindow("Test window", WINDOW_POSITION_X, WINDOW_POSITION_y,
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