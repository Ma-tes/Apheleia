#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include "../../src/engine.h"
#include "../../src/drawing/tile.h"
#include "entity.h"
#include "world_light.h"

#define WEAPONS_COUNT 3
#define WEAPONS_ANIMATION_COUNT 11

#define BULLET_BUFFER 4096
#define BULLET_SIZE (vector2) { 16, 16 }
#define BULLET_ANIMATION_COUNT 8

#define BULLET_EXPLOSION_SIZE (vector2) { 64, 64 }

#define INDEXED_ANIMATION_TILE(x, y)        \
    (tile_information) {                    \
        .index_position = { x, y },         \
        .type = BLOCK_TILE,                 \
        .draw_tile_f = draw_tile            \
    }                                       \

static tile_information bullet_tile = INDEXED_ANIMATION_TILE(0, 10);

typedef struct player_entity player_entity;

struct bullet {
    float velocity;

    bool is_active;
    animation_atlas *explosion_atlas;

    interactive_entity_t bullet_interactive_entity;
    player_entity *bullet_source_entity;
} bullets[BULLET_BUFFER];

static int bullets_count = 0;

enum weapon_type {
    PISTOL,
    RIFLE,
    SHOTGUN
};

typedef struct weapon {
    const char* name;
    enum weapon_type type;

    entity_t *weapon_entity;

    int spread_bullet_count;
    int bullet_maximum_count;

    int velocity_delay;
} weapon;

typedef struct player_entity {
    interactive_entity_t *base_interactive_entity;

    vector2 last_player_position;

    weapon *current_weapon;
    bool is_weapon_changed;

    uint health;
    uint score;

    uint lighting_charges_count;

    struct bullet *last_bullet;

    bool is_shooting;
    int bullets_fired_index;
} player_entity;

static struct {
    tile_information shoot_animation_frames[WEAPONS_ANIMATION_COUNT];

    tile_information top_shoot_animation_frames[WEAPONS_ANIMATION_COUNT];

} weapons_animation_frames[WEAPONS_COUNT] = {
    [PISTOL] = {
        .shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 16),
            INDEXED_ANIMATION_TILE(1, 16),
            INDEXED_ANIMATION_TILE(2, 16),
            INDEXED_ANIMATION_TILE(3, 16),
            INDEXED_ANIMATION_TILE(4, 16),
            INDEXED_ANIMATION_TILE(5, 16),
            INDEXED_ANIMATION_TILE(6, 16),
            INDEXED_ANIMATION_TILE(7, 16),
            INDEXED_ANIMATION_TILE(8, 16),
            INDEXED_ANIMATION_TILE(9, 16),
            INDEXED_ANIMATION_TILE(10, 16),
        },
        .top_shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 17),
            INDEXED_ANIMATION_TILE(1, 17),
            INDEXED_ANIMATION_TILE(2, 17),
            INDEXED_ANIMATION_TILE(3, 17),
            INDEXED_ANIMATION_TILE(4, 17),
            INDEXED_ANIMATION_TILE(5, 17),
            INDEXED_ANIMATION_TILE(6, 17),
            INDEXED_ANIMATION_TILE(7, 17),
            INDEXED_ANIMATION_TILE(8, 17),
            INDEXED_ANIMATION_TILE(9, 17),
            INDEXED_ANIMATION_TILE(10, 17),
        }
    },
    [RIFLE] = {
        .shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 20),
            INDEXED_ANIMATION_TILE(1, 20),
            INDEXED_ANIMATION_TILE(2, 20),
            INDEXED_ANIMATION_TILE(3, 20),
            INDEXED_ANIMATION_TILE(4, 20),
            INDEXED_ANIMATION_TILE(5, 20),
            INDEXED_ANIMATION_TILE(6, 20),
            INDEXED_ANIMATION_TILE(7, 20),
            INDEXED_ANIMATION_TILE(8, 20),
            INDEXED_ANIMATION_TILE(9, 20),
            INDEXED_ANIMATION_TILE(10, 20),
        },
        .top_shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 21),
            INDEXED_ANIMATION_TILE(1, 21),
            INDEXED_ANIMATION_TILE(2, 21),
            INDEXED_ANIMATION_TILE(3, 21),
            INDEXED_ANIMATION_TILE(4, 21),
            INDEXED_ANIMATION_TILE(5, 21),
            INDEXED_ANIMATION_TILE(6, 21),
            INDEXED_ANIMATION_TILE(7, 21),
            INDEXED_ANIMATION_TILE(8, 21),
            INDEXED_ANIMATION_TILE(9, 21),
            INDEXED_ANIMATION_TILE(10, 21),
        }
    },
    [SHOTGUN] = {
        .shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 18),
            INDEXED_ANIMATION_TILE(1, 18),
            INDEXED_ANIMATION_TILE(2, 18),
            INDEXED_ANIMATION_TILE(3, 18),
            INDEXED_ANIMATION_TILE(4, 18),
            INDEXED_ANIMATION_TILE(5, 18),
            INDEXED_ANIMATION_TILE(6, 18),
            INDEXED_ANIMATION_TILE(7, 18),
            INDEXED_ANIMATION_TILE(8, 18),
            INDEXED_ANIMATION_TILE(9, 18),
            INDEXED_ANIMATION_TILE(10, 18),
        },
        .top_shoot_animation_frames = {
            INDEXED_ANIMATION_TILE(0, 19),
            INDEXED_ANIMATION_TILE(1, 19),
            INDEXED_ANIMATION_TILE(2, 19),
            INDEXED_ANIMATION_TILE(3, 19),
            INDEXED_ANIMATION_TILE(4, 19),
            INDEXED_ANIMATION_TILE(5, 19),
            INDEXED_ANIMATION_TILE(6, 19),
            INDEXED_ANIMATION_TILE(7, 19),
            INDEXED_ANIMATION_TILE(8, 19),
            INDEXED_ANIMATION_TILE(9, 19),
            INDEXED_ANIMATION_TILE(10, 19),
        }
    }
};

static tile_information bullet_explosion_frames[BULLET_ANIMATION_COUNT] = {
    INDEXED_ANIMATION_TILE(17, 12),
    INDEXED_ANIMATION_TILE(18, 12),
    INDEXED_ANIMATION_TILE(19, 12),
    INDEXED_ANIMATION_TILE(20, 12),
    INDEXED_ANIMATION_TILE(21, 12),
    INDEXED_ANIMATION_TILE(22, 12),
    INDEXED_ANIMATION_TILE(23, 12),
    INDEXED_ANIMATION_TILE(24, 12),
};

static weapon weapons[WEAPONS_COUNT] = {
    [PISTOL] = (weapon) {
        .name = "Pistol",
        .type = PISTOL,
        .weapon_entity = &(entity_t) {
            .atlas = &(animation_atlas) {
                .type = FIRE_FORGET,
                .textures = weapons_animation_frames[PISTOL].shoot_animation_frames,
                .textures_count = WEAPONS_ANIMATION_COUNT,

                .color = (color) { 255, 255, 255 },
                .key_frame_speed = 50
            },
            .size = (vector2) { 64, 64 }
        },
        .velocity_delay = 400,
        .spread_bullet_count = 1,
        .bullet_maximum_count = -1
    },
    [RIFLE] = (weapon) {
        .name = "Rifle",
        .type = RIFLE,
        .weapon_entity = &(entity_t) {
            .atlas = &(animation_atlas) {
                .type = FIRE_FORGET,
                .textures = weapons_animation_frames[RIFLE].shoot_animation_frames,
                .textures_count = WEAPONS_ANIMATION_COUNT,

                .color = (color) { 255, 255, 255 },
                .key_frame_speed = 50
            },
            .size = (vector2) { 64, 64 }
        },
        .velocity_delay = 25,
        .spread_bullet_count = 1,
        .bullet_maximum_count = 32
    },
    [SHOTGUN] = (weapon) {
        .name = "Shotgun",
        .type = SHOTGUN,
        .weapon_entity = &(entity_t) {
            .atlas = &(animation_atlas) {
                .type = FIRE_FORGET,
                .textures = weapons_animation_frames[SHOTGUN].shoot_animation_frames,
                .textures_count = WEAPONS_ANIMATION_COUNT,

                .color = (color) { 255, 255, 255 },
                .key_frame_speed = 50
            },
            .size = (vector2) { 64, 64 }
        },
        .velocity_delay = 750,
        .spread_bullet_count = 5,
        .bullet_maximum_count = 8
    }
};

static tile_information remain_bullet_tile = INDEXED_ANIMATION_TILE(8, 0);
static vector2 remain_bullet_tile_size = (vector2) { 16, 16 };

void create_weapon_bullets(weapon relative_weapon, player_entity *relative_entity) {
    direction entity_direction = relative_entity->base_interactive_entity->base_direction;

    vector2 bullet_position = add(relative_entity->base_interactive_entity->base_entity.position,
        mul_value(directions[entity_direction], 5));

    vector2 direction_index = (vector2) { abs(directions[entity_direction].y), abs(directions[entity_direction].x) };
    bullet_position = add(bullet_position, mul_value(direction_index, 18 + (abs(directions[entity_direction].y) * 16)));

    bullets[bullets_count++] = (struct bullet) {
        .bullet_interactive_entity = (interactive_entity_t) {
            .base_entity = (entity_t) {
                .position = bullet_position,
                .collision_box_size = (vector2) { 18, 18 },
                .size = BULLET_SIZE,
                .texture = &bullet_tile
            },
            .base_direction = entity_direction,
            .is_entity_idle = false
        },
        .bullet_source_entity = relative_entity,
        .is_active = true
    };

    relative_entity->last_bullet = &(bullets[bullets_count - 1]);
    relative_entity->is_shooting = true;
    relative_entity->bullets_fired_index++;
}

void set_weapon_direction_animation(weapon *relative_weapon, direction relative_direction) {
    enum weapon_type current_weapon_type = relative_weapon->type;

    relative_weapon->weapon_entity->atlas->textures = relative_direction == NORTH || relative_direction == SOUTH ?
        weapons_animation_frames[current_weapon_type].top_shoot_animation_frames :
        weapons_animation_frames[current_weapon_type].shoot_animation_frames;
}

bool execute_weapon_fire(player_entity *entity) {
    if(entity->is_weapon_changed) {
        create_weapon_bullets(*entity->current_weapon, entity);
        entity->is_weapon_changed = false;

        return true;
    }

    vector2 last_bullet_position = entity->last_bullet->bullet_interactive_entity.base_entity.position;
    direction last_bullet_direction = entity->last_bullet->bullet_interactive_entity.base_direction;

    vector2 last_bullet_distance = (vector2) {
        .x = abs(last_bullet_position.x - entity->base_interactive_entity->base_entity.position.x),
        .y = abs(last_bullet_position.y - entity->base_interactive_entity->base_entity.position.y)
    };

    bool is_last_bullet_active = entity->last_bullet->is_active;

    last_bullet_distance = mul(last_bullet_distance, directions[last_bullet_direction]);

    if((bullets_count == 0 || !is_last_bullet_active || (abs(last_bullet_distance.x) > entity->current_weapon->velocity_delay ||
        abs(last_bullet_distance.y) > entity->current_weapon->velocity_delay))) {

        create_weapon_bullets(*entity->current_weapon, entity);
        return true;
    }
    return false;
}

void draw_linked_weapon(SDL_Renderer *renderer, player_entity *relative_entity) {
    weapon *relative_weapon = relative_entity->current_weapon;

    vector2 entity_position = relative_entity->base_interactive_entity->base_entity.position;
    direction entity_direction = relative_entity->base_interactive_entity->base_direction;

    relative_weapon->weapon_entity->position = add(entity_position, mul_value(directions[entity_direction], 5));

    relative_weapon->weapon_entity->position = entity_direction == NORTH || entity_direction == SOUTH ?
        add(relative_weapon->weapon_entity->position, (vector2) { 4, 0 }) :
        add(relative_weapon->weapon_entity->position, (vector2) { relative_entity->base_interactive_entity->base_entity.size.x / 4, 0});

    tile_external_information weapon_external = (tile_external_information) { entity_direction * 90, get_relative_texture_flip(*relative_entity->base_interactive_entity)};

    run_animation(relative_weapon->weapon_entity->atlas,
        renderer,
        relative_weapon->weapon_entity->position,
        relative_weapon->weapon_entity->size,
        &weapon_external);
    
    animation_atlas current_atlas = *relative_weapon->weapon_entity->atlas;

    if(relative_entity->is_shooting) {
        relative_weapon->weapon_entity->atlas->texture_index = 0;
        relative_entity->is_shooting = false;
    }
    else {
        draw_tile(renderer,
            current_atlas.textures[current_atlas.texture_index],
            relative_weapon->weapon_entity->position,
            colors[WHITE],
            relative_weapon->weapon_entity->size,
            &weapon_external);
    }
}

void draw_weapon_bullets(SDL_Renderer *renderer, player_entity player) {
    int current_bullets_count = player.current_weapon->bullet_maximum_count - player.bullets_fired_index;
    vector2 player_position = player.base_interactive_entity->base_entity.position;

    for (int i = 0; i < current_bullets_count; i++)
    {
        vector2 bullet_position = (vector2) {
            .x = player_position.x + (i * (remain_bullet_tile_size.x / 4)),
            .y = player_position.y - 24
        };

        draw_tile(renderer, remain_bullet_tile, bullet_position,
            colors[WHITE], remain_bullet_tile_size, NULL);
    }
}