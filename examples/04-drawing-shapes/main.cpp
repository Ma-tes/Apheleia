#define TILE_COUNT 0

#include <SDL2/SDL.h>
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "graph_class.hpp"
#include "cts_lib.h"
#include "lcd_lib.h"
#include "fonts.cpp"

tile_information tile_informations[TILE_COUNT] = {};
static RGB white_color = (RGB) { 255, 255, 255 };
static RGB black_color = (RGB) { 255, 255, 255 };

static cts_points_t *points_handler;

void initialization(global_engine_information *global) { }
void update(global_engine_information *global, float delta_time) { } 

void render(global_engine_information *global)
{
    FillRectangle header_rectangle((Point2D) {
      .x = 0,
      .y = 10 
    }, (Point2D) { 800, 300 }, (RGB) { 100, 150, 150 }, black_color, global->renderer);
    header_rectangle.draw();

    Rectangle rectangle((Point2D) {
      .x = 25,
      .y = 300 
    }, (Point2D) { 20, 100 }, (RGB) { 50, 50, 150 }, black_color, global->renderer);
    rectangle.draw();

    Text<256, 14> header_text((Point2D) {
        .x = 225,
        .y = 10 }, 12,
        "This is a DEMO program", g_font14x14, white_color, black_color, global->renderer);
    header_text.draw();

    Text<256, 8> information_text((Point2D) {
        .x = 10,
        .y = 40 }, 10,
        "Hello, you are currently in your DEMO program\nfully written by Apheleia library.", g_font8x8, white_color, black_color, global->renderer);
    information_text.draw();

    Circle testing_circle((Point2D) {
      .x = 300,
      .y = 400 }, 50,
      white_color, black_color, global->renderer);
    testing_circle.draw();

    if(cts_get_ts_points(points_handler, global) != 0)
    {

      const Point2D corners[4] = { { 0, 0 }, { 800, 0 }, { 0, 600}, { 800, 600 }};
      for (int i = 0; i < 4; i++)
      {
        Line corner_line(corners[i], (Point2D) {
            .x = points_handler->m_points[0].x,
            .y = points_handler->m_points[0].y,
          }, white_color, black_color, global->renderer);

        corner_line.draw();
      }
    }
}

int main()
{
    engine_state engine = (engine_state){
        .initialization_f = initialization,
        .update_f = update,
        .render_f = render,
    };

    global_engine_information engine_default_global = create_initialization_global(
        SDL_CreateWindow("Test window", 400, 0, 800, 600, SDL_WINDOW_OPENGL),
        800, 600);

    engine_default_global.framerate_limit = 120;
    engine.global_information = &engine_default_global;

    points_handler = (cts_points_t*)malloc(sizeof(cts_points_t));

    start_engine(&engine);
}