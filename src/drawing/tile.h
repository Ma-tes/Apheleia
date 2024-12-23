#pragma once

#include <SDL2/SDL.h>
#include "../math/vector.h"
#include "../drawing/color.h"
#include "../drawing/texture_manager.h"

//Default engine value representing maximum
//count of component handlers.
#if !defined(TILE_COUNT)
    #define TILE_COUNT 1
#endif

typedef enum tile_type {
    ENTITY_TILE = 1 << 1,
    BLOCK_TILE = 1 << 2,
    COLLISION_TILE = 1 << 3,
    ITEM_TILE = 1 << 4,
    ERROR_TILE = 1 << 5 
} tile_type;

typedef struct tile_information tile_information;
typedef struct tile_external_information tile_external_information;

typedef void (*draw_tile_function)(SDL_Renderer *renderer, const tile_information tile, vector2 position, color tile_color, vector2 size, tile_external_information *external);

typedef struct tile_information {
    char *name;
    int identificator;

    vector2 index_position;
    struct {
        int type;
        draw_tile_function draw_tile_f;
    };
} tile_information;

typedef struct tile_external_information {
    double angle;
    SDL_RendererFlip flip;
} tile_external_information;

extern tile_information tile_informations[TILE_COUNT];

tile_information *get_first_tile_by_type(int type) {
    for (int i = 0; i < TILE_COUNT; i++)
    {
        tile_information *current_tile = &tile_informations[i];
        if(current_tile->type == type) {
            return current_tile;
        }
    }
    return NULL;
}

void draw_tile(SDL_Renderer *renderer, const tile_information tile,
        vector2 position, color tile_color, vector2 size, const tile_external_information *external) {
    int max_texture_x_index = (int)(CURRENT_TEXTURE_ATLAS.size.x / CURRENT_TEXTURE_ATLAS.tile_offset_size.x);
    int max_texture_y_index = (int)(CURRENT_TEXTURE_ATLAS.size.y / CURRENT_TEXTURE_ATLAS.tile_offset_size.y);

    if(tile.index_position.x > max_texture_x_index || tile.index_position.x < 0 ||
        tile.index_position.y > max_texture_y_index || tile.index_position.y < 0) {
            tile_information *error_tile = get_first_tile_by_type(ERROR_TILE);
            if(error_tile != NULL) { error_tile->draw_tile_f(renderer, *error_tile, position, colors[WHITE], size, NULL); }
            return;
    }
    SDL_SetTextureColorMod(textures_storage.textures[current_texture_index].texture,
        tile_color.r,
        tile_color.g,
        tile_color.b);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    vector2 tile_position = (vector2) { 
        .x = tile.index_position.x * CURRENT_TEXTURE_ATLAS.tile_offset_size.x,
        .y = tile.index_position.y * CURRENT_TEXTURE_ATLAS.tile_offset_size.y
    };

    if(external == NULL) {
        render_texture_target(renderer, tile_position, position, size);
    }
    else {
        render_texture_target_ex(renderer, tile_position, position, size,
            external->angle, external->flip);
    }
}
