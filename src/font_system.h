#pragma once

#include <SDL2/SDL.h>
#include "math/vector.h"
#include "drawing/color.h"
#include "drawing/tile.h"

#define UNKNOWN_CHARACTER '?'
#define SPACE_CHARACTER ' '
#define NEW_LINE_CHARACTER '\n'

/**
 * A structure, that represents basic information holder
 * for any font "texture": Definied by series of tiles.
 * 
 * Field: `tile_count`: Is related to `font_pattern`,
 * which is used for position determination.
**/
typedef struct font_information {
    tile_information *font_tiles;
    SDL_Renderer *renderer;

    int tile_count;
    char *font_pattern;
} font_information;

void set_pattern_font_tiles(font_information *font, vector2 start_font_index, const int max_line_characters_count) {
    char *font_pattern = font->font_pattern;
    for (int i = 0; i < font->tile_count; i++)
    {
        char *font_character = (char*)malloc(sizeof(char) * 1); font_character[0] = font_pattern[i];
        vector2 relative_position = {
            .x = i % max_line_characters_count,
            .y = i / max_line_characters_count
        };

        font->font_tiles[i] = (tile_information){
            .name = font_character,
            .identificator = i,
            .index_position = add(start_font_index, relative_position)
        };
    }
}

void initializte_global_font(font_information *font, vector2 start_font_index, char *pattern, const int max_line_characters_count) {
    int count = strlen(pattern);
    tile_information *tiles = (tile_information*)malloc(sizeof(tile_information) * count);
    
    font->font_tiles = tiles;
    font->font_pattern = pattern;
    font->tile_count = count;

    set_pattern_font_tiles(font, start_font_index, max_line_characters_count);
}

//TODO: Create option to make global value,
//which has to definied.
void draw_char(const char character, vector2 position, color current_color, font_information *font, int size) {
    for (int i = 0; i < font->tile_count; i++)
    {
        tile_information current_tile = font->font_tiles[i];
        if(current_tile.name[0] == character) {
            draw_tile(font->renderer, current_tile, position, current_color, (vector2) { size, size });
            return;
        }
    }
    draw_char(UNKNOWN_CHARACTER, position, current_color, font, size);
}

void draw_text(const char* text, vector2 position, color current_color, font_information *font, int size) {
    int characters_count = strlen(text);
    int line_position_y = 0;

    int padding_index = 0;
    int index = 0;
    while(index < characters_count)
    {
        if(text[index] == NEW_LINE_CHARACTER) {
            line_position_y += size;
            padding_index = -1;
        }
        else if(text[index] != SPACE_CHARACTER) {
            draw_char(text[index], add(position, (vector2) { padding_index * size, line_position_y}), current_color, font, size);
        }
        index++;
        padding_index++;
    }
}

void draw_log_message(const char *text, const char *format, void* data, vector2 position, color current_color, font_information *font, int size) {
    char log_buffer[256];
    sprintf(log_buffer, format, data);

    draw_text(text, position, current_color, font, size);

    position.x += strlen(text) * size;
    draw_text(log_buffer, position, current_color, font, size);
}
