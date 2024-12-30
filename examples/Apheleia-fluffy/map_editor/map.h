#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../../../src/font_system.h"

#define WRITE_FILE_MODE "w"
#define READ_FILE_MODE "r"

enum serialization_messages {
    NONE_ERROR = 0,
    FILE_NOT_FOUND = 1,
    SERIALIZATION_ERROR = 2
};

typedef struct serialization_header {
    size_t tiles_count;
} serialization_header;

typedef struct map_tile {
    vector2 position;
    vector2 size;

    tile_information tile;
} map_tile;

typedef struct map_information {
    size_t count;
    map_tile *tiles;
} map_information;

int save_map_tiles(map_tile *tiles, size_t count, const char *path) {
    //Open file handler by path parameter
    FILE *file = fopen(path, WRITE_FILE_MODE);

    //Checks if file exists, if not it returns error message
    if(file == NULL) { return FILE_NOT_FOUND; }

    serialization_header header = { .tiles_count = count };
    fwrite(&header, sizeof(serialization_header), 1, file);

    //Writes related tiles data
    fwrite(tiles, sizeof(map_tile), count, file);

    //Closes stream link to related file
    fclose(file);

    return NONE;
}

map_information load_map_tiles(const char *path) {
    //Open file handler by path parameter
    FILE *file = fopen(path, READ_FILE_MODE);

    serialization_header header;
    fread(&header, sizeof(serialization_header), 1, file);

    map_tile *return_tiles = (map_tile*)malloc(sizeof(map_tile) * header.tiles_count);
    fread(return_tiles, sizeof(map_tile), header.tiles_count, file);

    return (map_information) {
        .count = header.tiles_count,
        .tiles = return_tiles
    };
}