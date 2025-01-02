#pragma once

#include "../map_editor/map.h"
#include "../../../src/math/vector.h"
#include "../../../src/math/random.h"

typedef struct entity entity_t;

map_tile get_random_tile(map_information map, enum tile_type type) {
    map_tile random_tile = map.tiles[random_range(0, map.count - 1)];
    while(random_tile.tile.type != type) {
        random_tile = map.tiles[random_range(0, map.count - 1)];
    }
    return random_tile;
}

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

vector2 *get_detection_positions(vector2 object_position, vector2 object_size, map_information map,
    const int range, const int offset, const int precision) {

    vector2 *detection_tiles = malloc(sizeof(vector2) * precision);
    for (int i = 0; i < precision; i++)
    {

        vector2 circle_value_position = {
            .x = cos(range * i) * (offset) + (object_position.x + (object_size.x / 2)),
            .y = sin(range * i) * (offset) + (object_position.y + (object_size.y / 2))
        };

        vector2 circle_tile_position = get_tile_position(circle_value_position, 64);
        detection_tiles[i] = circle_tile_position;
    }
    return detection_tiles;
}

map_information get_collision_tiles(vector2 object_position, vector2 object_size, map_information map,
        const int range, const int offset, const int precision) {

    map_tile *collision_tiles = malloc(sizeof(map_tile) * precision);
    int tile_index = 0;

    vector2 *object_detect_tiles = get_detection_positions(object_position, object_size, map, range, offset, precision);
    for (int i = 0; i < precision; i++)
    {
        int current_index = index_of_map_tile(map.tiles, map.count, object_detect_tiles[i]);
        if(current_index != -1 && map.tiles[current_index].tile.type == COLLISION_TILE) {
            collision_tiles[tile_index++] = map.tiles[current_index];
        }
    }
    return (map_information) {
        .tiles = collision_tiles,
        .count = tile_index
    };
}