#pragma once

#include "vector.h"
#include "color.h"
#include <SDL2/SDL.h>

#define CURRENT_TEXTURE_ATLAS textures_storage.textures[current_texture_index]

#if !defined(MAX_TEXTURES)
    #define MAX_TEXTURES 4
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
    if(position.x > CURRENT_TEXTURE_ATLAS.size.x || position.x < 0
        || position.y > CURRENT_TEXTURE_ATLAS.size.y || position.y < 0) {
        //TODO: Handle exception.
        return;
    }
    SDL_Rect texture_area = {
        .x = tile_position.x,
        .y = tile_position.y,
        .w = CURRENT_TEXTURE_ATLAS.tile_offset_size.x,
        .h = CURRENT_TEXTURE_ATLAS.tile_offset_size.y
    };

    SDL_Rect destination_texture_area = {
        .x = position.x,
        .y = position.y,
        .w = size.x,
        .h = size.y
    };

    SDL_RenderCopy(renderer, CURRENT_TEXTURE_ATLAS.texture, &texture_area, &destination_texture_area);
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
        textures_storage.textures = malloc(sizeof(texture_atlas_information) * MAX_TEXTURES);
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