#pragma once

#include <stdbool.h>
#include <time.h>
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/drawing/tile.h"
#include "entity.h"
#include "world_light.h"


static time_t start_time;
static time_t current_time;

typedef struct game_state_information {
    int game_time;
    int timer_seconds;

    int round_phase;
    int round_time;
} game_state_information;

static engine_state *apheleia_state;
static game_state_information *game_state;

static int collision_range = 16;
static int collision_offset = 64;
static int collision_precision = 64;

static lighting_configuration player_lighting_configuration = (lighting_configuration) {
    .range = 32,
    .offset = 1024,
    .precision = 64
};
