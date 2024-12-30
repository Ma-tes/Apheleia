#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../../../src/drawing/tile.h"
#include "../../../src/drawing/color.h"
#include "../../../src/engine_utilities/engine_global.h"
#include "../../../src/engine.h"
#include "../../../src/font_system.h"
#include "../apheleia_fluffy_global.h"
#include "../entity.h"
#include "../world_light.h"
#include "../world_light_actions.h"
#include "../tile_animator.h"
#include "../weapon_container.h"
#include "../map_editor/map.h"
#include "../utilities/tile_helper.h"
#include "../utilities/collision.h"

/**
 * Initialized infromations about tiles from loaded map.
**/

map_information map_tiles_information;
typedef struct direction_frames {
    tile_information animation_frames[4];
    int frames_limit_count;
} direction_frames;

static direction_frames idle_frames[4] = {
    [NORTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 2),
            INDEXED_ANIMATION_TILE(11, 2),
        },
        .frames_limit_count = 2
    },
    [SOUTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 0),
            INDEXED_ANIMATION_TILE(11, 0),
        },
        .frames_limit_count = 2
    },
    [WEST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 7),
            INDEXED_ANIMATION_TILE(11, 7),
        },
        .frames_limit_count = 2
    },
    [EAST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 1),
            INDEXED_ANIMATION_TILE(11, 1),
        },
        .frames_limit_count = 2
    },
};

static direction_frames running_frames[4] = {
    [NORTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 5),
            INDEXED_ANIMATION_TILE(11, 5),
            INDEXED_ANIMATION_TILE(12, 5),
            INDEXED_ANIMATION_TILE(13, 5),
        },
        .frames_limit_count = 4
    },
    [SOUTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 3),
            INDEXED_ANIMATION_TILE(11, 3),
            INDEXED_ANIMATION_TILE(12, 3),
            INDEXED_ANIMATION_TILE(13, 3),
        },
        .frames_limit_count = 4
    },
    [WEST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 6),
            INDEXED_ANIMATION_TILE(11, 6),
            INDEXED_ANIMATION_TILE(12, 6),
            INDEXED_ANIMATION_TILE(13, 6),
        },
        .frames_limit_count = 4
    },
    [EAST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 4),
            INDEXED_ANIMATION_TILE(11, 4),
            INDEXED_ANIMATION_TILE(12, 4),
            INDEXED_ANIMATION_TILE(13, 4),
        },
        .frames_limit_count = 4
    },
};

animation_atlas player_animation_configuration = (animation_atlas) {
    .type = REPEAT,

    .color = (color) { 255, 255, 255 },
    .key_frame_speed = 200
};

static player_entity player = (player_entity) {
    .last_player_position = (vector2) { 64, 64 },
    .base_interactive_entity = &(interactive_entity_t) {
        .base_entity = (entity_t) {
            .position = (vector2) { 64, 64 },
            .size = (vector2) { 48, 48 },
            .collision_box_offset = (vector2) { 8, 0 },
            .collision_box_size = (vector2) { 32, 48 },
            .atlas = &player_animation_configuration,
        },
        .base_direction = SOUTH,
        .is_entity_idle = false
    },

    .is_shooting = false, .is_weapon_changed = true,
    .lighting_charges_count = 5
};

enum { BULLETS, PLAYER };
light_state default_light_states[WORLD_LIGHT_ACTIONS_COUNT] = {
    [BULLETS] = (light_state) {
        .configuration = &(lighting_configuration) {
            .range = 4,
            .offset = 2,
            .precision = 4 
        },
        .tag = MULTIPLE_VOID,
        .action = bullet_lighting_action
    },
    [PLAYER] = (light_state) {
        .tag = SINGLE,
        .action = player_lighting_action
    }
};

entity_t get_bullet_entity(void *data) {
    struct bullet *relative_bullet = (struct bullet*)data;
    return relative_bullet->bullet_interactive_entity.base_entity;
}

void initialization_game_scene(global_engine_information *global) {
    /**
     * Thirdly we load our specified map, which is
     * deserialized into map_tile structure.
    **/
    map_tiles_information = load_map_tiles("map_editor/test.bin");


    default_light_states[PLAYER].position_entity = &(player.base_interactive_entity->base_entity);
    default_light_states[PLAYER].configuration = &player_lighting_configuration;

    default_light_states[BULLETS].anonymous_entities = (void**)malloc(sizeof(void*) * 1024);
    default_light_states[BULLETS].get_entity = &get_bullet_entity;

    game_state = (game_state_information*)malloc(sizeof(game_state_information));
    game_state->game_time = 120;
    game_state->round_time = 10;

    player.current_weapon = &weapons[game_configuration.weapon_index];

    player.base_interactive_entity->base_entity.color = game_configuration.player_color;
    player_animation_configuration.color = game_configuration.player_color;

    start_time = time(NULL);
}

void update_game_scene(global_engine_information *global, float delta_time) {
    /**
     * Use `SDL_GetKeyboardState` instead of `input_handler` for
     * any precise movement implementations.
    **/
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    player.last_player_position = player.base_interactive_entity->base_entity.position;

    if(keyboard_state[SDL_SCANCODE_W]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), NORTH, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.y -= 1.25 * delta_time;
        set_weapon_direction_animation(player.current_weapon, NORTH);
    }
    if(keyboard_state[SDL_SCANCODE_S]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), SOUTH, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.y += 1.25 * delta_time;
        set_weapon_direction_animation(player.current_weapon, SOUTH);
    }
    if(keyboard_state[SDL_SCANCODE_A]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), WEST, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.x -= 1.25 * delta_time;
        set_weapon_direction_animation(player.current_weapon, WEST);
    }
    if(keyboard_state[SDL_SCANCODE_D]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), EAST, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.x += 1.25 * delta_time;
        set_weapon_direction_animation(player.current_weapon, EAST);
    }

    if((player.base_interactive_entity->base_entity.position.x != player.last_player_position.x || 
        player.base_interactive_entity->base_entity.position.y != player.last_player_position.y)) {
            player.base_interactive_entity->is_entity_idle = false;
    }

    if(keyboard_state[SDL_SCANCODE_SPACE]) {
        if(player.current_weapon->bullet_maximum_count != -1 &&
            player.bullets_fired_index == player.current_weapon->bullet_maximum_count) {
                player.current_weapon = &weapons[PISTOL];
                player.bullets_fired_index = 0;
                player.is_weapon_changed = true;
        }
        else if(execute_weapon_fire(&player)) {
            default_light_states[BULLETS].anonymous_entities[default_light_states[BULLETS].count] =
                &(bullets[default_light_states[BULLETS].count]);
            default_light_states[BULLETS].count++;
        }
    }

    //Used only for debug purposes
    if(keyboard_state[SDL_SCANCODE_1]) { collision_offset++; }
    if(keyboard_state[SDL_SCANCODE_2]) { collision_offset--; }

    if(keyboard_state[SDL_SCANCODE_3]) { collision_precision++; }
    if(keyboard_state[SDL_SCANCODE_4]) { collision_precision--; }

    if(keyboard_state[SDL_SCANCODE_5]) { player_lighting_configuration.offset++; }
    if(keyboard_state[SDL_SCANCODE_6]) { player_lighting_configuration.offset--; }

    if(keyboard_state[SDL_SCANCODE_7]) { player_lighting_configuration.precision++; }
    if(keyboard_state[SDL_SCANCODE_8]) { player_lighting_configuration.precision--; }

    for (int i = 0; i < bullets_count; i++)
    {
        if(bullets[i].is_active) {
            bullets[i].velocity = bullets[i].velocity < 5.0f ? bullets[i].velocity + 0.8f : bullets[i].velocity + 0.0f;
            bullets[i].bullet_interactive_entity.base_entity.position = add(bullets[i].bullet_interactive_entity.base_entity.position, mul_value(directions[bullets[i].bullet_interactive_entity.base_direction], bullets[i].velocity * delta_time));
        }
    }

    time_t previous_time = current_time;
    current_time = time(NULL) - start_time;

    game_state->timer_seconds = game_state->game_time - current_time;

    if((current_time - previous_time) == 1 && current_time % game_state->round_time == 0) {
        game_state->round_phase++;
        player_lighting_configuration.offset = player_lighting_configuration.offset < 192 ?
            player_lighting_configuration.offset : player_lighting_configuration.offset - (128 - (game_state->round_phase * 4));
    }
}

void render_game_scene(global_engine_information *global) {
    set_light_detection_positions(&default_light_states[BULLETS], map_tiles_information);
    set_light_detection_positions(&default_light_states[PLAYER], map_tiles_information);

    for (int i = 0; i < map_tiles_information.count; i++)
    {
        map_tile current_tile = map_tiles_information.tiles[i];
        draw_tile(global->renderer, current_tile.tile,
            current_tile.position, colors[WHITE], current_tile.size, NULL);

        for (int j = 0; j < WORLD_LIGHT_ACTIONS_COUNT; j++)
        {
            draw_light_state(global->renderer, &default_light_states[j], current_tile);
        }

        last_active_light_positions = NULL;
        last_active_light_positions_count = 0;
    }

    vector2 *detection_tiles = get_detection_positions(
        player.base_interactive_entity->base_entity.position,
        player.base_interactive_entity->base_entity.size,
        map_tiles_information, collision_range, collision_offset, collision_precision);

    map_information collision_map = get_collision_tiles(
        player.base_interactive_entity->base_entity.position,
        player.base_interactive_entity->base_entity.size, map_tiles_information, collision_range, collision_offset, collision_precision);

    for (int i = 0; i < collision_precision; i++)
    {
        SDL_Rect detection_tile_rectangle = (SDL_Rect) {
            .x = detection_tiles[i].x-2 + TILE_DRAWING_OFFSET_X,
            .y = detection_tiles[i].y-2 + TILE_DRAWING_OFFSET_Y,
            .w = 68,
            .h = 68 
        };
        //DRAW_BORDER_RECTANGLE(global, &detection_tile_rectangle, colors[RED]);
    }

    for (int i = 0; i < collision_map.count; i++)
    {
        map_tile current_tile = collision_map.tiles[i];

        SDL_Rect detection_tile_rectangle = (SDL_Rect) {
            .x = current_tile.position.x-2 + TILE_DRAWING_OFFSET_X,
            .y = current_tile.position.y-2 + TILE_DRAWING_OFFSET_Y,
            .w = 68,
            .h = 68 
        };
        //DRAW_BORDER_RECTANGLE(global, &detection_tile_rectangle, colors[BLUE]);

        if(current_tile.tile.type == COLLISION_TILE && is_colliding(add(player.base_interactive_entity->base_entity.position,
            player.base_interactive_entity->base_entity.collision_box_offset), player.base_interactive_entity->base_entity.collision_box_size,
            current_tile.position, current_tile.size)) {

            player.base_interactive_entity->base_entity.position = player.last_player_position;
        }
    }

    free(detection_tiles);

    if((player.base_interactive_entity->base_entity.position.x == player.last_player_position.x &&
        player.base_interactive_entity->base_entity.position.y == player.last_player_position.y)) {
            if(!player.base_interactive_entity->is_entity_idle) {
                player_animation_configuration.textures = idle_frames[player.base_interactive_entity->base_direction].animation_frames;
                player_animation_configuration.textures_count = idle_frames[player.base_interactive_entity->base_direction].frames_limit_count;

                player_animation_configuration.texture_index = 0;
                player.base_interactive_entity->is_entity_idle = true;
            }
    }

    run_animation(player.base_interactive_entity->base_entity.atlas, global->renderer, player.base_interactive_entity->base_entity.position, player.base_interactive_entity->base_entity.size, NULL);
    player.last_player_position = player.base_interactive_entity->base_entity.position;

    draw_weapon_bullets(global->renderer, player);

    for (int i = 0; i < bullets_count; i++)
    {
        struct bullet current_bullet = bullets[i];
        vector2 bullet_position = current_bullet.bullet_interactive_entity.base_entity.position;

        if(current_bullet.bullet_source_entity == NULL) {
            continue;
        }

        if(!current_bullet.is_active) {
            run_animation(current_bullet.explosion_atlas, global->renderer, bullet_position,
                BULLET_EXPLOSION_SIZE, NULL);
            continue;
        }

        vector2 bullet_tile_position = get_tile_position(bullet_position, 64);
        
        int relative_tile_index = index_of_map_tile(map_tiles_information.tiles, map_tiles_information.count, bullet_tile_position);
        map_tile relative_tile = map_tiles_information.tiles[relative_tile_index];

        if(relative_tile.tile.type == COLLISION_TILE &&
            is_colliding(bullet_position, current_bullet.bullet_interactive_entity.base_entity.collision_box_size,
            relative_tile.position, relative_tile.size)) {

                bullets[i].explosion_atlas = malloc(sizeof(animation_atlas));

                bullets[i].explosion_atlas->type = FIRE_FORGET,
                bullets[i].explosion_atlas->textures = bullet_explosion_frames,
                bullets[i].explosion_atlas->textures_count = BULLET_ANIMATION_COUNT,

                bullets[i].explosion_atlas->color = (color) { 255, 255, 255 },
                bullets[i].explosion_atlas->key_frame_speed = 50;
                bullets[i].is_active = false;
        }
        else {
            tile_external_information bullet_external = (tile_external_information) {
                current_bullet.bullet_interactive_entity.base_direction * 90,
                get_relative_texture_flip(*player.base_interactive_entity)
            };

            draw_tile(global->renderer, *current_bullet.bullet_interactive_entity.base_entity.texture,
                current_bullet.bullet_interactive_entity.base_entity.position, colors[WHITE],
                    current_bullet.bullet_interactive_entity.base_entity.size, &bullet_external);
        }
    }
    draw_linked_weapon(global->renderer, &player);

    draw_log_message("Time: ", "%lu", (void*) (game_state->timer_seconds), (vector2) { 22, -50 }, colors[BLUE], global->font, 32);
    draw_log_message("Time: ", "%lu", (void*) (game_state->timer_seconds), (vector2) { 20, -47 }, colors[WHITE], global->font, 32);

    draw_log_message("Phase: ", "%lu", (void*) (game_state->round_phase), (vector2) { 390, -50 }, colors[BLUE], global->font, 32);
    draw_log_message("Phase: ", "%lu", (void*) (game_state->round_phase), (vector2) { 388, -47 }, colors[WHITE], global->font, 32);

    draw_log_message("Score: ", "%lu", (void*) (player.score), (vector2) { 720, -50 }, colors[BLUE], global->font, 32);
    draw_log_message("Score: ", "%lu", (void*) (player.score), (vector2) { 718, -47 }, colors[WHITE], global->font, 32);
}