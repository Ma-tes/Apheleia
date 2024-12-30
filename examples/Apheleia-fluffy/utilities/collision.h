#pragma once

#include <stdbool.h>
#include "../../../src/math/vector.h"

bool is_colliding(vector2 origin_position, vector2 origin_size,
    vector2 object_position, vector2 object_size) {
    bool collision_x = ((origin_position.x + origin_size.x) > object_position.x) &&
        ((origin_position.x) < (object_position.x + object_size.x));

    bool collision_y = ((origin_position.y + origin_size.y) > object_position.y) &&
        ((origin_position.y) < (object_position.y + object_size.y));

    return collision_x && collision_y;
}