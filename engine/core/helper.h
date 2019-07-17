#ifndef __ENGINE__CORE__HELPER__
#define __ENGINE__CORE__HELPER__

#include <string>
#include <allegro5/allegro5.h>

ALLEGRO_COLOR hex_string_to_allegro_color(const std::string);
int parse_relative_value(const char *src, int len, int default_value = 0);

char allegro_keycode_to_char(int keycode);

#endif