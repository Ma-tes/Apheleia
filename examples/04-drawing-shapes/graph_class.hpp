#pragma once

#include <SDL2/SDL.h>
#include "lcd_lib.h"

typedef struct Point2D 
{
    int32_t x, y;
} Point2D;

typedef struct RGB
{
    uint8_t r, g, b;
} RGB;

class GraphElement
{
    public:
        RGB m_fg_color, m_bg_color;
        SDL_Renderer *renderer;

        GraphElement(RGB t_fg_color, RGB t_bg_color, SDL_Renderer *renderer) : 
            m_fg_color(t_fg_color), m_bg_color(t_bg_color)
        {
            this->renderer = renderer;
        }

        void drawPixel(int32_t t_x, int32_t t_y)
        {
            lcd_put_pixel(t_x, t_y, convert_RGB888_to_RGB565(m_fg_color), renderer);
        }
        
        void draw() { }

        void hide()
        {
            swap_fg_bg_color();
            draw();
            swap_fg_bg_color();
        }

    private:
        void swap_fg_bg_color()
        {
            RGB l_tmp = m_fg_color;
            m_fg_color = m_bg_color;
            m_bg_color = l_tmp;
        } 

        uint16_t convert_RGB888_to_RGB565(RGB t_color)
        { 
            return (t_color.b & 31) |
                ((t_color.g & 63) << 5) |
                ((t_color.r & 31) << 11);
        }
};


class Pixel : public GraphElement
{
    public:
        Point2D m_pos;

        Pixel( Point2D t_pos, RGB t_fg_color, RGB t_bg_color, SDL_Renderer *renderer) : GraphElement(t_fg_color, t_bg_color, renderer), m_pos(t_pos)
        {
        }

    public: void draw()
    {
        drawPixel( m_pos.x, m_pos.y );
    }
};

class Rectangle : public GraphElement
{

    public:
        Point2D m_position;
        Point2D m_size;

        Rectangle(Point2D t_position, Point2D t_size, RGB t_fg, RGB t_bg, SDL_Renderer *renderer) :  
            GraphElement(t_fg, t_bg, renderer), m_position(t_position), m_size(t_size)
        {
        }

        void draw()
        {
            for (int i = 0; i < m_size.x; i++)
            {
                drawPixel(m_position.x + i, m_position.y);
                drawPixel(m_position.x + i, m_position.y + m_size.y);
            }

            for (int i = 0; i < m_size.y; i++)
            {
                drawPixel(m_position.x, m_position.y + i);
                drawPixel(m_position.x + m_size.x, m_position.y + i);
            }
        }
};

class FillRectangle : public GraphElement
{

    public:
        Point2D m_position;
        Point2D m_size;

        FillRectangle(Point2D t_position, Point2D t_size, RGB t_fg, RGB t_bg, SDL_Renderer *renderer) :  
            GraphElement(t_fg, t_bg, renderer), m_position(t_position), m_size(t_size)
        {
        }

        void draw()
        {
            for (int i = 0; i < m_size.y; i++)
            {
                for (int j = 0; j < m_size.x; j++)
                {
                    drawPixel(m_position.x + j, m_position.y + i);
                }
            }
        }
};

class Circle : public GraphElement
{

    public:
        Point2D m_center;
        int32_t m_radius;

        Circle(Point2D t_center, int32_t t_radius, RGB t_fg, RGB t_bg, SDL_Renderer *renderer) : 
            GraphElement(t_fg, t_bg, renderer), m_center(t_center), m_radius(t_radius)
        {
        }

        void draw()
        {
            int32_t error = -m_radius;
            int32_t x = m_radius;
            int32_t y = 0;
            
            while (x >= y)
            {
                plot8points (m_center.x, m_center.y, x, y);
                error += (2 * y) + 1;
                ++y;
                if (error >= 0)
                {
                    --x;
                    error -= 2 * x;
                }
            }
        }

    private: 
        void plot4points(int32_t cx, int32_t cy, int32_t x, int32_t y)
        {
            drawPixel(cx + x, cy + y);

            if (x != 0) { drawPixel(cx - x, cy + y); }
            if (y != 0) { drawPixel(cx + x, cy - y); }

            if (x != 0 && y != 0) { drawPixel(cx - x, cy - y); }
        }

        void plot8points(int32_t cx, int32_t cy, int32_t x, int32_t y)
        {
            plot4points (cx, cy, x, y);
            if (x != y) { plot4points (cx, cy, y, x); }
        }
};

template <size_t font_count, size_t font_lenght>
class Character : public GraphElement 
{
    private:
        uint8_t (&font_characters)[font_count][font_lenght];

    public:
        Point2D m_pos;
        char m_character;

        Character( Point2D t_pos, char t_char, uint8_t (&font_characters)[font_count][font_lenght], RGB t_fg, RGB t_bg, SDL_Renderer *renderer) :
            GraphElement(t_fg, t_bg, renderer), m_pos(t_pos), font_characters(font_characters), m_character(t_char)
        {
        };

        void draw()
        {
            auto font_structure = (font_characters[(uint8_t)m_character]);
            for (int i = 0; i < font_lenght; i++)
            {
                uint8_t font_portion = font_structure[i];
                for (int j = 0; j < font_lenght; j++)
                {
                    if((font_portion >> j) & 0x01)
                    {
                        drawPixel(m_pos.x + j, m_pos.y + i);
                    }
                }
            }
        }
};

#define MAX_TEXT_BUFFER 128
template <size_t font_count, size_t font_lenght>
class Text : public GraphElement
{
    public:
        Point2D m_pos;
        u_int16_t m_spacing;
        Character<font_count, font_lenght> **m_characters;
        u_int32_t m_characters_count;

        Text(Point2D t_pos, u_int16_t t_spacing, const char *t_text, uint8_t (&font_characters)[font_count][font_lenght],
            RGB t_fg, RGB t_bg, SDL_Renderer *renderer) : GraphElement(t_fg, t_bg, renderer), m_pos(t_pos), m_spacing(t_spacing)
        {
            m_characters = (Character<font_count, font_lenght>**)malloc(sizeof(Character<font_count, font_lenght>*) * MAX_TEXT_BUFFER);
            create_m_characters(t_text, font_characters);
        };

        void draw()
        {
            for (int i = 0; i < m_characters_count; i++)
            {
                m_characters[i]->draw();
            }
        }

    private:
        void create_m_characters(const char *t_text, uint8_t (&font_characters)[font_count][font_lenght])
        {
            uint32_t current_index = 0;
            uint32_t final_character_count = 0;

            int32_t current_x_position = 0;
            int32_t current_y_position = 0;
            while(t_text[current_index] != '\0')
            {
                if(t_text[current_index] == '\n')
                {
                    current_x_position = 0;
                    current_y_position++; current_index++;
                    continue;
                }

                Point2D relative_position = { m_pos.x + current_x_position, m_pos.y + (current_y_position * m_spacing) };
                m_characters[final_character_count] = new Character<font_count, font_lenght>(relative_position, t_text[current_index], font_characters,
                    m_fg_color, m_bg_color, renderer);

                current_x_position += m_spacing;
                current_index++; final_character_count++;
            }

            m_characters_count = final_character_count;
        }
};

class Line : public GraphElement
{
    public:
        Point2D m_pos1, m_pos2;

        Line(Point2D t_pos1, Point2D t_pos2, RGB t_fg, RGB t_bg, SDL_Renderer *renderer) :
            GraphElement(t_fg, t_bg, renderer), m_pos1(t_pos1), m_pos2(t_pos2)
        {
        }

        void draw()
        {
            Point2D start = m_pos1;
            Point2D end = m_pos2;

            int xDifference = abs(end.x - start.x);
            int yDifference = abs(end.y - start.y);

            if(xDifference == 0 || yDifference == 0)
            {
                draw_straight_line();
            }

            int xIndexer = start.x < end.x ? 1 : -1;
            int yIndexer = start.y < end.y ? 1 : -1;

            int gradientError = (xDifference > yDifference ? xDifference : -yDifference) / 2;
            while(start.x != end.x && start.y != end.y)
            {
                drawPixel(start.x, start.y);

                int currentGradientError = gradientError;

                if(currentGradientError > -xDifference)
                {
                    gradientError -= yDifference;
                    start.x += xIndexer; 
                }
                if(currentGradientError < yDifference)
                {
                    gradientError += xDifference;
                    start.y += yIndexer; 
                }
            }
        }

    private:
        void draw_straight_line()
        {
            int differenceX = m_pos2.x - m_pos1.x;
            int indexerX = differenceX == 0 ? 0 : (differenceX > 0 ? 1 : -1);

            int differenceY = m_pos2.y - m_pos1.y;
            int indexerY = differenceY == 0 ? 0 : (differenceY > 0 ? 1 : -1);

            Point2D difference = (Point2D)
            {
                .x = (abs)(indexerX * (m_pos1.x - m_pos2.x)),
                .y = (abs)(indexerY * (m_pos1.y - m_pos2.y))
            };

            for (int i = 0; i < difference.x + difference.y; i++)
            {
                drawPixel(m_pos1.x + (indexerX * i), m_pos1.y + (indexerY * i));
            }
        }
};
