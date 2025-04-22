#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct RGB_lcd {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_lcd;

RGB_lcd convert_RGB565_to_RGB888(uint16_t t_color)
{
    return (RGB_lcd) {
        .r = (uint8_t)((t_color >> 11) << 3),
        .g = (uint8_t)((t_color >> 5) << 2),
        .b = (uint8_t)((t_color) << 3)
    };
}
 
void lcd_put_pixel(uint16_t t_x, uint16_t t_y, uint16_t t_color, SDL_Renderer *renderer)
{
    RGB_lcd base_color = convert_RGB565_to_RGB888(t_color);

    SDL_SetRenderDrawColor(renderer,
        base_color.r,
        base_color.g,
        base_color.b, 255);
    SDL_RenderDrawPoint(renderer, t_x, t_y);
}

void lcd_put_square(uint16_t t_x, uint16_t t_y, uint16_t t_color, uint8_t t_size_8, SDL_Renderer *renderer)
{
    for (int i = 0; i < t_size_8; i++)
    {
        for (int j = 0; j < t_size_8; j++)
        {
            lcd_put_pixel(t_x + i, t_y + j, t_color, renderer);
        }
    }
}