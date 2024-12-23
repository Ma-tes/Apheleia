/**
 *  FLUFFY MAP EDITOR
 * 
 * It shows the use of Apheleia engine in a
 * single file. It uses all components which
 * Apheleia could offer, with many useful implementations:
 *  1. Collisions, for UI elements.
 *  2. Interactive changing sprites.
 *  3. Input handling, with `input_handler` and with `SDL_GetKeyboardState`.
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

#include "../../../src/drawing/color.h"

#define TILE_COUNT 78 
#define SDL_CLEAR_COLOR (color) { .r = 244, .g = 245, .b = 249 }

#include <SDL2/SDL.h>
#include "../../../src/drawing/tile.h"
#include "../../../src/engine_utilities/engine_global.h"
#include "../../../src/engine.h"
#include "../../../src/font_system.h"
#include "../utilities/collision.h"
#include "../utilities/tile_helper.h"
#include "map.h"

/**
 * Character representation of font atlas in file: "bin/main_texture_sheet.bmp".
**/
#define FONT_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890,.+-*/=()[]{}?!:;%%"

#define TEST_TEXTURE_PATH "../../../bin/test_sprites_3.0.bmp"

/**
 * Color representation of font atlas key color, for creating transparent tiles.
**/
#define TEXTURE_ATLAS_KEY_COLOR (color) { 159, 159, 159 }

/**
 * Fixed width of window, used in global initialization.
**/
#define WINDOW_WIDTH 1600

/**
 * Fixed height of window, used in global initialization.
**/
#define WINDOW_HEIGHT 1200

/**
 * Fixed position x of window, used in global initialization.
**/
#define WINDOW_POSITION_X 0

/**
 * Fixed position y of window, used in global initialization.
**/
#define WINDOW_POSITION_y 0

#define MAP_WIDTH 1024
#define MAP_HEIGHT 960

#define MAP_POSITION_X 256
#define MAP_POSITION_Y 128

#define MOD(x, y) ((x) < (0)) ? ((y) - ((-(x)) % y)) : (x) % (y)

#define DRAW_RECTANGLE(global, rectangle, rectangle_color)          \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderFillRect(global->renderer, rectangle);                \

#define DRAW_BORDER_RECTANGLE(global, rectangle, rectangle_color)   \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderDrawRect(global->renderer, rectangle);                \

#define PANEL_COLOR (color) { .r = 252, .g = 252, .b = 252 }

#define TEXT_LIGHT_COLOR (color) { .r = 175, .g = 175, .b = 175 }
#define TEXT_DARK_COLOR (color) { .r = 90, .g = 90, .b = 100 }

#define TOOL_COUNT 4

static int current_tile_index = 0;
static enum tile_type current_tile_type = BLOCK_TILE;

int map_tile_index = 0;
int tool_index = 0;
bool is_tile_place = false;

int global_tile_size = 64;

static SDL_Rect map_rectangle = (SDL_Rect) {
    .x = MAP_POSITION_X,
    .y = MAP_POSITION_Y,
    .w = MAP_WIDTH,
    .h = MAP_HEIGHT
};

tile_information tile_informations[TILE_COUNT];

static map_tile map_tiles[(MAP_WIDTH / 8) * (MAP_HEIGHT / 8)];

int remove_block_indexes[(MAP_WIDTH / 8) * (MAP_HEIGHT / 8)];
int remove_block_count = 0;

enum { PAINT, REMOVE, CLEAR, SAVE };
void paint_tool_action(global_engine_information*, vector2);
void remove_tool_action(global_engine_information*, vector2);
void clear_tool_action(global_engine_information*, vector2);
void save_tool_action(global_engine_information*, vector2);

struct {
    SDL_Keycode code;
    char *name;
    void(*tool_action)(global_engine_information*, vector2);
} editor_tools[TOOL_COUNT] = {
    [PAINT] = {
        .code = SDL_SCANCODE_P,
        .name = "P - Paint",
        .tool_action = paint_tool_action
    },
    [REMOVE] = {
        .code = SDL_SCANCODE_R,
        .name = "R - Remove",
        .tool_action = remove_tool_action
    },
    [CLEAR] = {
        .code = SDL_SCANCODE_O,
        .name = "O - Clear",
        .tool_action = clear_tool_action
    },
    [SAVE] = {
        .code = SDL_SCANCODE_I,
        .name = "I - Save",
        .tool_action = save_tool_action
    },
};

void set_editor_tiles(const int start_index, const int tile_row_count) {
    for (int i = 0; i < TILE_COUNT; i++)
    {
        vector2 tile_position = {
            .x = (start_index + i) % tile_row_count,
            .y = (start_index + i) / tile_row_count
        };
        tile_informations[i] = (tile_information) {
            .name = "--Pre-generated-tile--",
            .identificator = start_index + i,
            .index_position = tile_position,
            .type = BLOCK_TILE,
            .draw_tile_f = draw_tile,
        };
    }
}

int get_current_tool_index(input *input_handler) {
    for (int i = 0; i < TOOL_COUNT; i++)
    {
        if(input_handler->buttons[editor_tools[i].code].is_key_pressed) {
            return i;
        }
    }
    return -1;
}
 
void set_position_tile(const vector2 tile_position, const vector2 tile_size, tile_information *tile, const int index) {
    int current_map_tile_index = index_of_map_tile(map_tiles, current_tile_index, tile_position);
    if(current_map_tile_index != -1) {
        map_tiles[current_map_tile_index].size = tile_size;
        map_tiles[current_map_tile_index].tile = *tile;
        return;
    }
    
    map_tiles[index] = (struct map_tile){
        .position = tile_position,
        .size = tile_size,
        .tile = *tile
    };
}

void paint_tool_action(global_engine_information *global, vector2 mouse_position) {
    vector2 relative_tile_position = sub(mouse_position, (vector2) { MAP_POSITION_X, MAP_POSITION_Y });
    tile_informations[current_tile_index].draw_tile_f(global->renderer, tile_informations[current_tile_index],
        mouse_position, colors[WHITE], (vector2) { global_tile_size, global_tile_size }, NULL);

    int tile_index = index_of_map_tile(map_tiles, map_tile_index, relative_tile_position);
    if(tile_index != -1 && map_tiles[tile_index].tile.draw_tile_f != NULL &&
        map_tiles[tile_index].tile.type == COLLISION_TILE) {

        map_tiles[tile_index].tile.draw_tile_f(global->renderer, map_tiles[tile_index].tile,
            mouse_position, colors[BLUE], (vector2) { global_tile_size, global_tile_size }, NULL);
    }

   bool is_tile_same = map_tiles[tile_index].tile.identificator == tile_informations[current_tile_index].identificator;
   if(is_tile_place && !is_tile_same) {
        tile_information *current_tile = malloc(sizeof(tile_information));
        current_tile->name = tile_informations[current_tile_index].name;
        current_tile->identificator = tile_informations[current_tile_index].identificator;
        current_tile->index_position = tile_informations[current_tile_index].index_position;
        current_tile->draw_tile_f = tile_informations[current_tile_index].draw_tile_f;

        current_tile->type = (int)current_tile_type;

        set_position_tile(relative_tile_position, (vector2) { global_tile_size, global_tile_size },
            current_tile, remove_block_count > 0 ? remove_block_count-- : map_tile_index++);
    }

    if(is_tile_place && is_tile_same && map_tiles[tile_index].tile.type != (int)current_tile_type) {
        map_tiles[tile_index].tile.type = (int)current_tile_type;
    }
}

void remove_tool_action(global_engine_information *global, vector2 mouse_position) {
    vector2 relative_tile_position = sub(mouse_position, (vector2) { MAP_POSITION_X, MAP_POSITION_Y });
    int tile_index = index_of_map_tile(map_tiles, map_tile_index, relative_tile_position);

    if(tile_index != -1 && map_tiles[tile_index].tile.draw_tile_f != NULL) {
        map_tiles[tile_index].tile.draw_tile_f(global->renderer, map_tiles[tile_index].tile,
            mouse_position, colors[RED], map_tiles[tile_index].size, NULL);

        if(is_tile_place) {
            remove_block_indexes[remove_block_count++] = tile_index;
            //free(map_tiles[tile_index].tile);

            map_tiles[tile_index] = (struct map_tile) {
                .position = ZERO_VECTOR2,
                .size = ZERO_VECTOR2,
                .tile = (NULL)
            };
        }
    }
}

void clear_tool_action(global_engine_information *global, vector2 mouse_position) {
    memset(map_tiles, 0, (MAP_WIDTH / 8) * (MAP_HEIGHT / 8));
    map_tile_index = 0;
    remove_block_count = 0;
}

void save_tool_action(global_engine_information *global, vector2 mouse_position) {
    save_map_tiles(map_tiles, map_tile_index, "test.bin");
    tool_index = 0;
}

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
        .size = (vector2) { .x = 1024, .y = 1024 },
        .tile_offset_size = (vector2) { .x = 64, .y = 64 }
    });

    set_editor_tiles(32, 10);
}

void update(global_engine_information *global, float delta_time) {
    /**
     * Use `SDL_GetKeyboardState` instead of `input_handler` for
     * any precise movement implementations.
    **/
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    is_tile_place = keyboard_state[SDL_SCANCODE_SPACE];

    if(global->input_handler->buttons[SDL_SCANCODE_UP].is_key_pressed) {
        current_tile_index = MOD(current_tile_index - 1, TILE_COUNT);
    }
    if(global->input_handler->buttons[SDL_SCANCODE_DOWN].is_key_pressed) {
        current_tile_index = MOD(current_tile_index + 1, TILE_COUNT);
    }

    if(global->input_handler->buttons[SDL_SCANCODE_N].is_key_pressed) {
        current_tile_type = BLOCK_TILE;
    }
    if(global->input_handler->buttons[SDL_SCANCODE_C].is_key_pressed) {
        current_tile_type = COLLISION_TILE;
    }

    int current_tool_index = get_current_tool_index(global->input_handler);
    tool_index = current_tool_index != - 1 ? current_tool_index : tool_index;
}

static SDL_Rect title_rectangle = (SDL_Rect) { .x = MAP_POSITION_X - 8, .y = 16, .w = MAP_WIDTH, .h = 64 };
static SDL_Rect title_rectangle_shadow = (SDL_Rect) { .x = MAP_POSITION_X + 8, .y = 32, .w = MAP_WIDTH, .h = 64 };

static SDL_Rect tool_rectangle = (SDL_Rect) { .x = (MAP_POSITION_X + MAP_WIDTH) + 32, .y = 0, .w = WINDOW_WIDTH - ((MAP_POSITION_X + MAP_WIDTH) + 32), .h = WINDOW_HEIGHT };
static SDL_Rect tile_rectangle = (SDL_Rect) { .x = 0, .y = 0, .w = MAP_POSITION_X - 32, .h = WINDOW_HEIGHT };

void render(global_engine_information *global) {
    DRAW_RECTANGLE(global, &map_rectangle, PANEL_COLOR)
    DRAW_RECTANGLE(global, &tool_rectangle, PANEL_COLOR)
    DRAW_RECTANGLE(global, &tile_rectangle, PANEL_COLOR)

    DRAW_RECTANGLE(global, &title_rectangle_shadow, colors[GRAY])
    DRAW_RECTANGLE(global, &title_rectangle, colors[DARK_GRAY])

    draw_text("Apheleia editor", (vector2) { MAP_POSITION_X + (MAP_WIDTH / 3), 32 }, colors[WHITE], global->font, 32);

    draw_text("N - Normal block", (vector2) { tool_rectangle.x + 32, 30 },
        current_tile_type == COLLISION_TILE ? colors[BLACK] : colors[GREEN], global->font, 12);
    draw_text("C - Collision block", (vector2) { tool_rectangle.x + 32, 60 },
        current_tile_type == COLLISION_TILE ? colors[GREEN] : colors[BLACK], global->font, 12);

    int current_tile_y_position = 32;
    for (int i = 0; i < TILE_COUNT; i++)
    {
        if(i % 4 == 0) { current_tile_y_position = (i + 1) * 12; }
        vector2 tile_position = (vector2) { (32 + ((i % 4) * 40)), (MAP_POSITION_Y + 32) + current_tile_y_position };
        if(current_tile_index == i) {
            SDL_Rect select_tile_rectangle = (SDL_Rect) {
                .x = tile_position.x-4,
                .y = tile_position.y-4,
                .w = 40,
                .h = 40
            };
            DRAW_RECTANGLE(global, &select_tile_rectangle, colors[GREEN])
        }

        tile_informations[i].draw_tile_f(global->renderer, tile_informations[i],
            tile_position, colors[WHITE], (vector2) { 32, 32 }, NULL);
    }

    for (int i = 0; i < TOOL_COUNT; i++)
    {
        vector2 current_position = (vector2) { tool_rectangle.x + 32, (MAP_POSITION_Y + tool_rectangle.y) + ((i + 1) * 32)};
        color current_color = i == tool_index ? colors[GREEN] : colors[BLACK];

        draw_text(editor_tools[i].name, current_position, current_color, global->font, 16);
    }


    for (int i = 0; i < map_tile_index; i++)
    {
        if(map_tiles[i].tile.draw_tile_f != NULL) {
            tile_information current_tile = map_tiles[i].tile;

            vector2 current_position = map_tiles[i].position;
            vector2 relative_position = add(current_position, (vector2) { MAP_POSITION_X, MAP_POSITION_Y });

            if(current_tile_type == COLLISION_TILE && current_tile.type == COLLISION_TILE) {
                SDL_Rect select_tile_rectangle = (SDL_Rect) {
                    .x = relative_position.x-2,
                    .y = relative_position.y-2,
                    .w = 68,
                    .h = 68 
                };
                DRAW_RECTANGLE(global, &select_tile_rectangle, colors[BLUE])
            }
            current_tile.draw_tile_f(global->renderer, current_tile,
                relative_position, colors[WHITE], (vector2) { global_tile_size, global_tile_size }, NULL);
        }
    }

    //TODO: Move tile rectangle calculation to update..
    SDL_Rect tile_rectangle = (SDL_Rect) {
        .x = (int)global->input_handler->cursor.position.x,
        .y = (int)global->input_handler->cursor.position.y,
        .w = 1,
        .h = 1
    };

    //TODO: Move tile mouse position calculation to update..
    vector2 tile_mouse_position = get_tile_position((vector2) { tile_rectangle.x, tile_rectangle.y }, global_tile_size);

    const vector2 map_position = (vector2) { MAP_POSITION_X, MAP_POSITION_Y };
    const vector2 map_size = (vector2) { MAP_WIDTH, MAP_HEIGHT };

    if(is_colliding(global->input_handler->cursor.position, (vector2) { 1, 1 }, map_position, map_size)) {
        vector2 relative_tile_position = sub(tile_mouse_position, map_position);

        draw_log_message("x: ", "%lu", (void*)(u_long)relative_tile_position.x,
            sub(tile_mouse_position, (vector2) { 0, 24 }), colors[DARK_GRAY], global->font, 8);
        draw_log_message("y: ", "%lu", (void*)(u_long)relative_tile_position.y,
            sub(tile_mouse_position, (vector2) { 0, 12 }), colors[DARK_GRAY], global->font, 8);

        if(editor_tools[tool_index].tool_action != NULL) {
            editor_tools[tool_index].tool_action(global, tile_mouse_position);
        }
    }
}

int main() {
    engine_state engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    global_engine_information engine_default_global = create_initialization_global(
        SDL_CreateWindow("Apheleia fluffy editor", WINDOW_POSITION_X, WINDOW_POSITION_y,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL
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

    start_engine(&engine);
}