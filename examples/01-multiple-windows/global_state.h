#pragma once

#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WINDOW_POSITION_X 400 
#define WINDOW_POSITION_y 0

#define LOGGING_WINDOW_WIDTH 400 
#define LOGGING_WINDOW_HEIGHT 1200

#define LOGGING_WINDOW_POSITION_X 0
#define LOGGING_WINDOW_POSITION_y 0

#define TEST_TEXTURE_PATH "../../bin/main_texture_sheet.bmp"

//Global declarations of global_engine_information
//for logging purposes.
static global_engine_information engine_default_global;
static global_engine_information logging_engine_default_global;

static engine_state engine;
static engine_state logging_engine;