#pragma once

#include "../map_editor/map.h"
#include "../../../src/math/vector.h"

vector2 get_tile_position(vector2 position, int tile_size) {
    return (vector2) {
        .x = tile_size * (((int)position.x) / tile_size),
        .y = tile_size * (((int)position.y) / tile_size)
    };
}

int index_of_map_tile(map_tile *map_tiles, int count, const vector2 tile_position) {
    for (int i = 0; i < count; i++)
    {
        vector2 map_tile_position = map_tiles[i].position;
        if(tile_position.x == map_tile_position.x && tile_position.y == map_tile_position.y) {
            return i;
        }
    }
    return -1;
}