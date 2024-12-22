#pragma once

#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/math/vector.h"
#include "tile_animator.h"

typedef struct entity {
    vector2 position;
    vector2 size;

    vector2 collision_box_offset;
    vector2 collision_box_size;

    color color;

    union {
        tile_information *texture;
        animation_atlas *atlas;
    };
} entity;