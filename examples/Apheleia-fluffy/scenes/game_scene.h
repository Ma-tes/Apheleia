#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../../../src/drawing/tile.h"
#include "../../../src/drawing/color.h"
#include "../../../src/math/random.h"
#include "../../../src/engine_utilities/engine_global.h"
#include "../../../src/engine.h"
#include "../../../src/font_system.h"
#include "../entity.h"
#include "../enemy_entity.h"
#include "../world_light.h"
#include "../world_light_actions.h"
#include "../tile_animator.h"
#include "../weapon_container.h"
#include "../map_editor/map.h"
#include "../utilities/tile_helper.h"
#include "../utilities/collision.h"
#include "../apheleia_fluffy_global.h"

#define PLAYER_SPAWN_POSITION (vector2) { 64, 64 }

/**
 * Initialized infromations about tiles from loaded map.
**/

map_information map_tiles_information;

player_entity player = (player_entity) {
    .base_interactive_entity = &(interactive_entity_t) {
        .base_entity = (entity_t) {
            .size = (vector2) { 48, 48 },
            .collision_box_offset = (vector2) { 8, 0 },
            .collision_box_size = (vector2) { 32, 48 },
            .atlas = &player_animation_configuration,
        },
        .base_direction = SOUTH,
        .is_entity_idle = false
    },
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

void create_random_weapon_item(void) {
    random_weapon_item.current_weapon_type = (enum weapon_type)random_range(0, WEAPONS_COUNT - 1);
    random_weapon_item.spawn_position = get_random_tile(map_tiles_information, BLOCK_TILE).position;
    random_weapon_item.visualization_position = random_weapon_item.spawn_position;
    random_weapon_item.spawn_time = game_state->timer_seconds;
    random_weapon_item.is_collected = false;
}

entity_t get_bullet_entity(void *data) {
    struct bullet *relative_bullet = (struct bullet*)data;
    return relative_bullet->bullet_interactive_entity.base_entity;
}

void initialization_game_scene(global_engine_information *global) {
    map_tiles_information = load_map_tiles(map_paths[game_configuration.map_index]);

    game_state = (game_state_information*)malloc(sizeof(game_state_information));
    game_state->game_time = 120;
    game_state->round_time = 10;
    game_state->player_light = player_lighting_configuration;

    default_light_states[PLAYER].position_entity = &(player.base_interactive_entity->base_entity);
    default_light_states[PLAYER].configuration = &player_lighting_configuration;

    player.health = 3;
    player.last_player_position = PLAYER_SPAWN_POSITION;
    player.base_interactive_entity->base_entity.position = PLAYER_SPAWN_POSITION;
    player.current_weapon = &weapons[game_configuration.weapon_index];

    player.base_interactive_entity->base_entity.color = game_configuration.player_color;
    player_animation_configuration.color = game_configuration.player_color;

    player.is_shooting = false;
    player.is_weapon_changed = true;
    player.bullets_fired_index = 0;
    player.total_bullets_fired = 0;
    player.lighting_charges_count = 5;

    random_weapon_item.current_weapon_type = ERROR_TILE;
    random_weapon_item.visualization_size = (vector2) { 48, 48 };

    enemy_entities = (enemy_entity**)malloc(sizeof(enemy_entity*) * ENTITIES_COUNT);
    global_entities = (player_entity**)malloc(sizeof(player_entity*) * (ENTITIES_COUNT + 1));
    entities_index = 0;

    global_entities[0] = &player;

    player.weapon_atlas = (animation_atlas*)malloc(sizeof(animation_atlas));

    player.weapon_atlas->type = FIRE_FORGET;
    player.weapon_atlas->textures = weapons[game_configuration.weapon_index].weapon_entity->atlas->textures;
    player.weapon_atlas->textures_count = WEAPONS_ANIMATION_COUNT;

    player.weapon_atlas->color = (color) { 255, 255, 255 };
    player.weapon_atlas->key_frame_speed = 50;

    create_bullets_memory();
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
        set_weapon_direction_animation(&player, NORTH);
    }
    if(keyboard_state[SDL_SCANCODE_S]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), SOUTH, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.y += 1.25 * delta_time;
        set_weapon_direction_animation(&player, SOUTH);
    }
    if(keyboard_state[SDL_SCANCODE_A]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), WEST, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.x -= 1.25 * delta_time;
        set_weapon_direction_animation(&player, WEST);
    }
    if(keyboard_state[SDL_SCANCODE_D]) {
        EXECUTE_DIRECTION_ANIMATION((*player.base_interactive_entity), EAST, player_animation_configuration, running_frames)

        player.base_interactive_entity->base_entity.position.x += 1.25 * delta_time;
        set_weapon_direction_animation(&player, EAST);
    }

    if(global->input_handler->buttons[SDL_SCANCODE_ESCAPE].is_key_pressed) {
        current_scene_identificator = PAUSE_SCENE;
    }

    if(keyboard_state[SDL_SCANCODE_5]) { player_lighting_configuration.offset++; }
    if(keyboard_state[SDL_SCANCODE_6]) { player_lighting_configuration.offset--; }

    if(keyboard_state[SDL_SCANCODE_7]) { player_lighting_configuration.precision++; }
    if(keyboard_state[SDL_SCANCODE_8]) { player_lighting_configuration.precision--; }

    if((player.base_interactive_entity->base_entity.position.x != player.last_player_position.x || 
        player.base_interactive_entity->base_entity.position.y != player.last_player_position.y)) {
            player.base_interactive_entity->is_entity_idle = false;
    }

    if(keyboard_state[SDL_SCANCODE_SPACE]) {
        if(player.current_weapon->bullet_maximum_count != -1 &&
            player.bullets_fired_index == player.current_weapon->bullet_maximum_count) {
                player.current_weapon = &weapons[PISTOL];
                player.weapon_atlas->textures = weapons[PISTOL].weapon_entity->atlas->textures;

                player.bullets_fired_index = 0;
                player.is_weapon_changed = true;
        }
        execute_weapon_fire(&player);
    }

    for (int i = 0; i < entities_index; i++)
    {
        enemy_entity current_enemy = *enemy_entities[i];
        if(current_enemy.base_player_entity.health == 0) {
            free_path_node(current_enemy.path);
            free_path_node(current_enemy.current_path_node);
            continue;
        }

        vector2 enemy_tile_position = get_tile_position(current_enemy.base_player_entity.base_interactive_entity->base_entity.position, 64);

        if(current_enemy.current_path_node != NULL && equal(enemy_tile_position, current_enemy.destination_position)) {
            current_enemy.path = NULL;
            current_enemy.current_path_node = NULL;
            current_enemy.last_path_node = NULL;

            current_enemy.is_following_path = false;
            current_enemy.moves_count = 0;
        }

        if(current_enemy.path == NULL) {
            if(generate_enemy_destination(&current_enemy, global_entities, entities_index, map_tiles_information)) {
                free_path_node(current_enemy.path);
                free_path_node(current_enemy.current_path_node);

                generate_enemy_move_tiles(&current_enemy, map_tiles_information, NULL);
                current_enemy.current_path_node = current_enemy.path;
            }
        }

        if(current_enemy.path != NULL) {
            current_enemy.base_player_entity.base_interactive_entity->is_entity_idle = false;
            follow_path_node(global, &current_enemy, delta_time);
        }

        if(current_enemy.current_path_node != NULL) {
            vector2 node_tile_position = get_tile_position(current_enemy.current_path_node->position, 64);
            if(equal(enemy_tile_position, node_tile_position)) {
                current_enemy.last_path_node = current_enemy.current_path_node;
                current_enemy.current_path_node = current_enemy.current_path_node->next_node;
            }
        }
    }

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

    if(game_state->timer_seconds == 0) {
        current_scene_identificator = SCORE_SCENE;
        engine_scenes[GAME_SCENE].execute_initialization = true;
    }

    if((current_time - previous_time) == 1 && current_time % 2 == 0) {
        enemy_entities[entities_index] = create_random_enemy(map_tiles_information);
        global_entities[entities_index + 1] = &enemy_entities[entities_index]->base_player_entity;
        entities_index++;
    }

    if((current_time - previous_time) == 1 && current_time % game_state->round_time == 0) {
        game_state->round_phase++;

        player_lighting_configuration.offset = player_lighting_configuration.offset < 192 ?
            player_lighting_configuration.offset : player_lighting_configuration.offset - (128 - (game_state->round_phase * 4));
    }

    if((current_time - previous_time) == 1 && current_time % weapon_item_spawn_length == 0) {
        create_random_weapon_item();
    }

    random_weapon_item.visualization_position = add(random_weapon_item.visualization_position,
        (vector2) { 0, SDL_clamp((sin(current_time) * delta_time), -0.1, 0.1) });
}

void render_game_scene(global_engine_information *global) {
    //set_light_detection_positions(&default_light_states[BULLETS], map_tiles_information);

    for (int i = 0; i < map_tiles_information.count; i++)
    {
        map_tile current_tile = map_tiles_information.tiles[i];
        draw_tile(global->renderer, current_tile.tile,
            current_tile.position, colors[WHITE], current_tile.size, NULL);
    }

    for (int i = 0; i < entities_index; i++)
    {
        enemy_entity *current_enemy = enemy_entities[i];
        if(current_enemy->base_player_entity.health == 0) {
            continue;
        }

        if(!current_enemy->is_following_path && !current_enemy->base_player_entity.base_interactive_entity->is_entity_idle) {
            direction node_direction = current_enemy->base_player_entity.base_interactive_entity->base_direction;

            current_enemy->base_player_entity.base_interactive_entity->base_entity.atlas->textures = idle_frames[node_direction].animation_frames;
            current_enemy->base_player_entity.base_interactive_entity->base_entity.atlas->textures_count = idle_frames[node_direction].frames_limit_count;

            current_enemy->base_player_entity.base_interactive_entity->base_entity.atlas->texture_index = 0;
            current_enemy->base_player_entity.base_interactive_entity->is_entity_idle = true;
        }

        if(current_enemy->base_player_entity.base_interactive_entity->base_entity.atlas->textures != NULL) {
            run_animation(current_enemy->base_player_entity.base_interactive_entity->base_entity.atlas,
                global->renderer, current_enemy->base_player_entity.base_interactive_entity->base_entity.position,
                    current_enemy->base_player_entity.base_interactive_entity->base_entity.size, NULL);
        }

        draw_player_health(global->renderer, current_enemy->base_player_entity);
        draw_weapon_bullets(global->renderer, current_enemy->base_player_entity);

        draw_linked_weapon(global->renderer, &current_enemy->base_player_entity);
        visualize_path_nodes(global, current_enemy->path);
    }

    set_light_detection_positions(&default_light_states[PLAYER], map_tiles_information);
    for (int i = 0; i < map_tiles_information.count; i++)
    {
        map_tile current_tile = map_tiles_information.tiles[i];
        for (int j = 0; j < WORLD_LIGHT_ACTIONS_COUNT; j++)
        {
            draw_light_state(global->renderer, &default_light_states[j], current_tile);
        }

        last_active_light_positions = NULL;
        last_active_light_positions_count = 0;
    }

    map_information collision_map = get_collision_tiles(
        player.base_interactive_entity->base_entity.position,
        player.base_interactive_entity->base_entity.size, map_tiles_information, collision_range, collision_offset, collision_precision);

    for (int i = 0; i < collision_map.count; i++)
    {
        map_tile current_tile = collision_map.tiles[i];
        if(current_tile.tile.type == COLLISION_TILE && is_colliding(add(player.base_interactive_entity->base_entity.position,
            player.base_interactive_entity->base_entity.collision_box_offset), player.base_interactive_entity->base_entity.collision_box_size,
            current_tile.position, current_tile.size)) {

            player.base_interactive_entity->base_entity.position = player.last_player_position;
        }
    }

    free(collision_map.tiles);

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

    for (int i = 0; i < entities_index + 1; i++)
    {
        player_entity *current_player = global_entities[i];
        if(current_player->health == 0) {
            continue;
        }
        if(random_weapon_item.current_weapon_type != ERROR_TILE && !random_weapon_item.is_collected) {
            if(is_colliding(add(current_player->base_interactive_entity->base_entity.position,
                current_player->base_interactive_entity->base_entity.collision_box_offset), current_player->base_interactive_entity->base_entity.collision_box_size,
                random_weapon_item.visualization_position, random_weapon_item.visualization_size)) {

                    current_player->current_weapon = &weapons[random_weapon_item.current_weapon_type];
                    current_player->weapon_atlas->textures = weapons[random_weapon_item.current_weapon_type].weapon_entity->atlas->textures;

                    current_player->bullets_fired_index = 0;
                    current_player->is_weapon_changed = true;

                    random_weapon_item.is_collected = true;
            }

            float y_weapon_offset = random_weapon_item.visualization_size.y / 3;

            vector2 weapon_timer_position = add(random_weapon_item.visualization_position,
                (vector2) { (random_weapon_item.visualization_size.x / 2) - 8, y_weapon_offset * -1 });
            vector2 weapon_animation_position = add(random_weapon_item.visualization_position,
                (vector2) { (random_weapon_item.visualization_size.x / 4) * -1 , y_weapon_offset });
            
            run_animation(&weapon_spawn_animation_atlas, global->renderer, weapon_animation_position,
                (vector2) { 64, 64 }, NULL);

            int weapon_time = weapon_item_spawn_length - (random_weapon_item.spawn_time - game_state->timer_seconds);
            draw_log_message("", "%lu", (void*)(weapon_time), weapon_timer_position, colors[DARK_GRAY], global->font, 16);

            draw_tile(global->renderer,
                weapons[random_weapon_item.current_weapon_type].weapon_entity->atlas->textures[0],
                random_weapon_item.visualization_position,
                colors[WHITE], random_weapon_item.visualization_size, NULL);
        }
    }

    draw_player_health(global->renderer, player);
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

        for (int j = 0; j < entities_index + 1; j++)
        {
            player_entity *current_player = global_entities[j];
            if(current_player->health == 0) {
                continue;
            }

            bool bullet_colliding = is_colliding(add(current_player->base_interactive_entity->base_entity.position,
            current_player->base_interactive_entity->base_entity.collision_box_offset), current_player->base_interactive_entity->base_entity.collision_box_size,
                bullet_position, current_bullet.bullet_interactive_entity.base_entity.collision_box_size);

            if(current_bullet.bullet_source_entity->identificator != current_player->identificator && bullet_colliding) {
                current_player->health--;
                bullets[i].bullet_source_entity->score += bullet_score_points;

                bullets[i].explosion_atlas = malloc(sizeof(animation_atlas));

                bullets[i].explosion_atlas->type = FIRE_FORGET,
                bullets[i].explosion_atlas->textures = bullet_explosion_frames,
                bullets[i].explosion_atlas->textures_count = BULLET_ANIMATION_COUNT,

                bullets[i].explosion_atlas->color = (color) { 255, 255, 255 },
                bullets[i].explosion_atlas->key_frame_speed = 50;
                bullets[i].is_active = false;
            }
        }

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