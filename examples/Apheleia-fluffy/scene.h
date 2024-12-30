#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../../src/engine.h"
#include "../../src/engine_utilities/engine_global.h"

#if !defined(SCENE_COUNT) 
    #define SCENE_COUNT 1
#endif

typedef struct scene {
    const char *identificator_name;
    int identificator;

    bool execute_initialization;

    initialization_function initialization;
    update_function update;
    render_function render;
} scene;

extern scene engine_scenes[SCENE_COUNT];

void execute_scene_initialization(scene *relative_scene, global_engine_information *global) {
    if(!relative_scene->execute_initialization) {
        return;
    }

    relative_scene->initialization(global);
    relative_scene->execute_initialization = false;
}

void execute_scene_update(scene *relative_scene, global_engine_information *global, float delte_time) {
    if(relative_scene->execute_initialization) {
        relative_scene->initialization(global);
        relative_scene->execute_initialization = false;
    }

    relative_scene->update(global, delte_time);
}

void execute_scene_render(scene *relative_scene, global_engine_information *global) {
    if(relative_scene->execute_initialization) {
        relative_scene->initialization(global);
        relative_scene->execute_initialization = false;
    }

    relative_scene->render(global);
}