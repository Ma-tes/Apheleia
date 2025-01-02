#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include "../../src/engine.h"
#include "../../src/drawing/tile.h"
#include "../../src/math/random.h"
#include "../../src/engine_utilities/engine_global.h"
#include "entity.h"
#include "utilities/tile_helper.h"
#include "weapon_container.h"
#include "apheleia_fluffy_global.h"

#define DRAW_BORDER_RECTANGLE(global, rectangle, rectangle_color)   \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderDrawRect(global->renderer, rectangle);                \

#define DIRECTION_MOVE_PATH(enemy, map, origin_path_node, direction)                                                            \
    (direction_bias) {                                                                                                          \
        .relative_direction = (direction),                                                                                      \
        .relative_bias_tile = map.tiles[index_of_map_tile(map.tiles, map.count,                                                 \
            get_tile_position(add(origin_path_node->position, mul_value(directions[(direction)], 64)), 64))],                   \
        .bias = distance(add(origin_path_node->position, mul_value(directions[(direction)], 64)), enemy->destination_position)  \
    }                                                                                                                           \

#define ENTITIES_COUNT 512

static int enemy_current_identificator = 0;

typedef struct enemy_configuration {
    uint detection_radius;
    uint speed;
} enemy_configuration;

typedef struct path_node path_node; 

struct path_node {
    vector2 position;
    direction relative_direction;

    path_node *next_node;
};

typedef struct direction_bias {
    direction relative_direction;
    map_tile relative_bias_tile;

    float bias;
} direction_bias;

typedef struct enemy_entity {
    player_entity base_player_entity;

    vector2 destination_position;

    int moves_count;

    path_node *path;

    path_node *last_path_node;
    path_node *current_path_node;

    bool is_following_path;
} enemy_entity;

static enemy_entity **enemy_entities;
static player_entity **global_entities;

static int entities_index = 0;

enemy_entity *create_random_enemy(map_information map) {
    vector2 enemy_position = get_random_tile(map, BLOCK_TILE).position;
    enum weapon_type enemy_weapon = (enum weapon_type)random_range(0, WEAPONS_COUNT - 1);

    animation_atlas *enemy_atlas = (animation_atlas*)malloc(sizeof(animation_atlas));
    animation_atlas *weapon_atlas = (animation_atlas*)malloc(sizeof(animation_atlas));

    interactive_entity_t *interactive_enemy = (interactive_entity_t*)malloc(sizeof(interactive_entity_t));
    player_entity *player_enemy = (player_entity*)malloc(sizeof(player_entity));
    enemy_entity *enemy = (enemy_entity*)malloc(sizeof(enemy_entity));

    enemy_atlas->type = REPEAT;
    enemy_atlas->color = (color) { 255, 255, 255 };
    enemy_atlas->key_frame_speed = 200;

    weapon_atlas->type = FIRE_FORGET;
    weapon_atlas->textures_count = WEAPONS_ANIMATION_COUNT;
    weapon_atlas->textures = weapons[enemy_weapon].weapon_entity->atlas->textures;

    weapon_atlas->color = (color) { 255, 255, 255 };
    weapon_atlas->key_frame_speed = 50;

    interactive_enemy->base_entity = (entity_t) {
        .position = enemy_position,
        .size = (vector2) { 48, 48 },
        .collision_box_offset = (vector2) { 8, 0 },
        .collision_box_size = (vector2) { 32, 48 },
        .atlas = enemy_atlas,
    };

    interactive_enemy->base_direction = SOUTH;
    interactive_enemy->is_entity_idle = false;

    player_enemy->base_interactive_entity = interactive_enemy;
    player_enemy->current_weapon = &weapons[enemy_weapon];
    player_enemy->health = 3;
    player_enemy->identificator = enemy_current_identificator++;
    player_enemy->is_weapon_changed = true;
    player_enemy->weapon_atlas = weapon_atlas;

    enemy->base_player_entity = *player_enemy;
    enemy->is_following_path = false;

    return enemy;
}

bool generate_enemy_destination(enemy_entity *enemy, player_entity **entities, int count, map_information map) {
    double spawn_weapon_distance = distance(enemy->base_player_entity.base_interactive_entity->base_entity.position, random_weapon_item.visualization_position);

    for (int i = 0; i < count; i++)
    {
        vector2 entity_position = entities[i]->base_interactive_entity->base_entity.position;
        if(enemy->base_player_entity.identificator != entities[i]->identificator && distance(entity_position, enemy->base_player_entity.base_interactive_entity->base_entity.position) < 120) {
            enemy->destination_position = get_tile_position(entity_position, 64);
            //execute_weapon_fire(&enemy->base_player_entity);
            return true;
        }
    }

    if(random_weapon_item.current_weapon_type != ERROR_TILE && !random_weapon_item.is_collected
        && spawn_weapon_distance < 500) {

        vector2 weapon_tile_position = get_tile_position(random_weapon_item.spawn_position, 64);
        enemy->destination_position = weapon_tile_position;
        return true;
    }

    return false;
}

void generate_enemy_move_tiles(enemy_entity *enemy, map_information map, path_node *origin_path_node) {
    if(origin_path_node == NULL) {
        enemy->path = (path_node*)malloc(sizeof(path_node));
        origin_path_node = enemy->path;
        origin_path_node->position = enemy->base_player_entity.base_interactive_entity->base_entity.position;
    }

    if(equal(origin_path_node->position, enemy->destination_position) || enemy->moves_count > 1000) {
        return;
    }

    direction_bias directions_bias[4] = {
        DIRECTION_MOVE_PATH(enemy, map, origin_path_node, NORTH),
        DIRECTION_MOVE_PATH(enemy, map, origin_path_node, SOUTH),
        DIRECTION_MOVE_PATH(enemy, map, origin_path_node, WEST),
        DIRECTION_MOVE_PATH(enemy, map, origin_path_node, EAST)
    };

    int lowest_direction_index = 0;
    for (int i = 0; i < 4; i++)
    {
        if((directions_bias[i].bias < directions_bias[lowest_direction_index].bias
            && directions_bias[i].relative_bias_tile.tile.type != COLLISION_TILE)
            || directions_bias[lowest_direction_index].relative_bias_tile.tile.type == COLLISION_TILE) {

                lowest_direction_index = i;
        }
    }

    path_node *next_path_node = (path_node*)malloc(sizeof(path_node));
    next_path_node->position = directions_bias[lowest_direction_index].relative_bias_tile.position;

    enemy->moves_count++;
    generate_enemy_move_tiles(enemy, map, next_path_node);

    origin_path_node->next_node = next_path_node;
    origin_path_node->relative_direction = directions_bias[lowest_direction_index].relative_direction;
}
 
void visualize_path_nodes(global_engine_information *global, path_node *node) {
    if(node == NULL) {
        return;
    }

    SDL_Rect detection_tile_rectangle = (SDL_Rect) {
        .x = node->position.x-2 + TILE_DRAWING_OFFSET_X,
        .y = node->position.y-2 + TILE_DRAWING_OFFSET_Y,
        .w = 68,
        .h = 68 
    };
    DRAW_BORDER_RECTANGLE(global, &detection_tile_rectangle, colors[GREEN]);

    visualize_path_nodes(global, node->next_node);
    //free(node);
}

void follow_path_node(global_engine_information *global, enemy_entity *enemy, float delta_time) {
    if(enemy->current_path_node == NULL) {
        enemy->is_following_path = false;
        return;
    }

    direction node_direction = enemy->last_path_node == NULL ?
        enemy->current_path_node->relative_direction :
        enemy->last_path_node->relative_direction;

    EXECUTE_DIRECTION_ANIMATION((*enemy->base_player_entity.base_interactive_entity), (node_direction), *(enemy->base_player_entity.base_interactive_entity->base_entity.atlas), running_frames)

    enemy->base_player_entity.base_interactive_entity->base_entity.position = add(enemy->base_player_entity.base_interactive_entity->base_entity.position,
        mul_value(directions[node_direction], 1.25 * delta_time));

    set_weapon_direction_animation(&enemy->base_player_entity, node_direction);

    enemy->is_following_path = true;
}
 
void free_path_node(path_node *node) {
    if(node == NULL) {
        return;
    }
    free_path_node(node->next_node);
    free(node);
}