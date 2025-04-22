#pragma once

#include <SDL2/SDL.h>
#include "../math/vector.h"
#include "../drawing/color.h"

#define CURRENT_TEXTURE_ATLAS textures_storage.textures[current_texture_index]

#if !defined(MAX_TEXTURES)
    #define MAX_TEXTURES 4
#endif

#if !defined(TILE_DRAWING_OFFSET_X)
    #define TILE_DRAWING_OFFSET_X 0 
#endif

#if !defined(TILE_DRAWING_OFFSET_Y)
    #define TILE_DRAWING_OFFSET_Y 0
#endif

enum image_file_type {
    PNG,
    JPG,
    BMP
};

typedef SDL_Surface* (*load_surface_f)(SDL_RWops *src, int freesrc);

load_surface_f file_types[3] = {
    [BMP] = SDL_LoadBMP_RW,
    [PNG] = SDL_LoadBMP_RW,
    [JPG] = SDL_LoadBMP_RW,
};

typedef struct texture_atlas_information {
    SDL_Texture *texture;
    vector2 size;
    vector2 tile_offset_size;
} texture_atlas_information;

static struct {
    texture_atlas_information* textures;
    int count;
} textures_storage;

static int current_texture_index = 0;

void render_texture_target(SDL_Renderer *renderer, vector2 tile_position, vector2 position, vector2 size) {
    SDL_Rect texture_area = {
        .x = (int)tile_position.x,
        .y = (int)tile_position.y,
        .w = (int)CURRENT_TEXTURE_ATLAS.tile_offset_size.x,
        .h = (int)CURRENT_TEXTURE_ATLAS.tile_offset_size.y
    };

    SDL_Rect destination_texture_area = {
        .x = TILE_DRAWING_OFFSET_X + (int)position.x,
        .y = TILE_DRAWING_OFFSET_Y + (int)position.y,
        .w = (int)size.x,
        .h = (int)size.y
    };

    SDL_RenderCopy(renderer, CURRENT_TEXTURE_ATLAS.texture, &texture_area, &destination_texture_area);
}

void render_texture_target_ex(SDL_Renderer *renderer, vector2 tile_position,
    vector2 position, vector2 size, const double angle, const SDL_RendererFlip flip) {
    SDL_Rect texture_area = {
        .x = (int)tile_position.x,
        .y = (int)tile_position.y,
        .w = (int)CURRENT_TEXTURE_ATLAS.tile_offset_size.x,
        .h = (int)CURRENT_TEXTURE_ATLAS.tile_offset_size.y
    };

    SDL_Rect destination_texture_area = {
        .x = TILE_DRAWING_OFFSET_X + (int)position.x,
        .y = TILE_DRAWING_OFFSET_Y + (int)position.y,
        .w = (int)size.x,
        .h = (int)size.y
    };

    SDL_RenderCopyEx(renderer, CURRENT_TEXTURE_ATLAS.texture, &texture_area, &destination_texture_area,
        angle, NULL, flip);
}

SDL_Texture* create_texture(const char* path, enum image_file_type type, SDL_Renderer *renderer,
        const color transparent_key_color) {
    SDL_RWops *rwops = SDL_RWFromFile(path, "rb");
    SDL_Surface *surface = file_types[type](rwops, 1);

    Uint32 transparent_key_map = SDL_MapRGB(surface->format,
        transparent_key_color.r,
        transparent_key_color.g,
        transparent_key_color.b);
    SDL_SetColorKey(surface, SDL_TRUE, transparent_key_map);

    if(surface == NULL) {
        //TODO: Handle exception.
    }
    SDL_Texture *return_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return return_texture;
}

void add_texture_atlas(texture_atlas_information texture_atlas) {
    if(textures_storage.textures == NULL) {
        textures_storage.textures = (texture_atlas_information*)malloc(sizeof(texture_atlas_information) * MAX_TEXTURES);
    }

    textures_storage.textures[textures_storage.count] = texture_atlas;
    textures_storage.count++;
}

void free_texture_manager() {
    for (int i = 0; i < textures_storage.count; i++)
    {
        SDL_DestroyTexture(textures_storage.textures[i].texture);
    }
}