#include "core/color.h"

ALLEGRO_COLOR Color::TRANSPARENT;
ALLEGRO_COLOR Color::WHITE;
ALLEGRO_COLOR Color::BLACK;
ALLEGRO_COLOR Color::RED;
ALLEGRO_COLOR Color::GREEN;
ALLEGRO_COLOR Color::BLUE;
ALLEGRO_COLOR Color::YELLOW;
ALLEGRO_COLOR Color::LIGHT_GRAY;
ALLEGRO_COLOR Color::GRAY;
ALLEGRO_COLOR Color::GOLD;
ALLEGRO_COLOR Color::BROWN;
ALLEGRO_COLOR Color::PALE_TURQUOISE;
ALLEGRO_COLOR Color::PALE_GOLDENROD;

void Color::init()
{
    Color::TRANSPARENT = al_map_rgba(0, 0, 0, 0);
    Color::WHITE = al_map_rgb(255, 255, 255);
    Color::BLACK = al_map_rgb(0, 0, 0);
    Color::RED = al_map_rgb(255, 0, 0);
    Color::GREEN = al_map_rgb(0, 255, 0);
    Color::BLUE = al_map_rgb(0, 0, 255);
    Color::YELLOW = al_map_rgb(255, 255, 0);
    Color::LIGHT_GRAY = al_map_rgb(211, 211, 211);
    Color::GRAY = al_map_rgb(190, 190, 190);
    Color::GOLD = al_map_rgb(255, 215, 0);
    Color::BROWN = al_map_rgb(165, 42, 42);
    Color::PALE_TURQUOISE = al_map_rgb(175, 238, 238);
    Color::PALE_GOLDENROD = al_map_rgb(240, 230, 140);
}