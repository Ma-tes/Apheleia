#include <SDL2/SDL.h>
#include "tile.h"
#include "color.h"
#include "engine.h"
#include "font_system.h"
#include "engine_global.h"
 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_POSITION_X 400 
#define WINDOW_POSITION_y 0

#define LOGGING_WINDOW_WIDTH 400 
#define LOGGING_WINDOW_HEIGHT 1200

#define LOGGING_WINDOW_POSITION_X 0
#define LOGGING_WINDOW_POSITION_y 0

#define TEST_TEXTURE_PATH "../bin/main_texture_sheet.bmp"

vector2 sprites_position = ZERO_VECTOR2;
enum test_tiles { FIRST, SECOND, THIRD, FOURTH, ERROR};

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

//Global declarations of global_engine_information
//for logging purposes.
static global_engine_information engine_default_global;
static global_engine_information logging_engine_default_global;

static engine_state engine;
static engine_state logging_engine;

void initialization(global_engine_information *global) {
    SDL_Texture *current_texture = create_texture(TEST_TEXTURE_PATH, BMP, global->renderer,
        (color) { 159, 159, 159 });

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
        SDL_CreateWindow(
            "Test window",
            WINDOW_POSITION_X, WINDOW_POSITION_y,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL
        ), WINDOW_WIDTH, WINDOW_HEIGHT
    );

    logging_engine_default_global = create_initialization_global(
        SDL_CreateWindow(
            "Logging window",
            LOGGING_WINDOW_POSITION_X, LOGGING_WINDOW_POSITION_y,
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
        },
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]?!:%%", 16);

    logging_engine_default_global.font = (font_information*)malloc(sizeof(font_information));
    initializte_global_font(logging_engine_default_global.font,
        (vector2) {
            .x = 0,
            .y = 0
        },
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]?!:%%", 16);

    //start_engine(&engine);
    //start_engine(&logging_engine);
    engine_state *engines[2] = { &engine, &logging_engine };
    start_engines(engines, 2);
}