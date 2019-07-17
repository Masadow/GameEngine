#include <cmath>
#include "core/helper.h"
#include <iostream>


ALLEGRO_COLOR hex_string_to_allegro_color(const std::string color_str)
{
    unsigned int color_int = std::stoul(color_str, 0, 16);
    return color_str.size() == 6 ?
        al_map_rgb(color_int >> 16 & 255, color_int >> 8 & 255, color_int & 255):
        al_map_rgba(color_int >> 16 & 255, color_int >> 8 & 255, color_int & 255, color_int >> 24);

}

int parse_relative_value(const char *src, int len, int default_value)
{
    if (!src)
    {
        return default_value;
    }
    std::string str(src);
    bool from_right = false;
    if (str.front() == '-')
    {
        str = str.substr(1);
        from_right = true;
    }
    if (str.back() == '%')
    {
        str.pop_back();
        float factor = stof(str) / 100;
        return round(len * (from_right ? 1 - factor : factor));
    }
    char *p;
    int val = std::strtol(str.c_str(), &p, 10);
    return *p ? default_value : (from_right ? len - val : val);
}

char allegro_keycode_to_char(int keycode)
{
    if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z)
    {
        return 'a' + (keycode - ALLEGRO_KEY_A);
    }
    if (keycode >= ALLEGRO_KEY_0 && keycode <= ALLEGRO_KEY_9)
    {
        return '0' + (keycode - ALLEGRO_KEY_0);
    }
    return keycode == ALLEGRO_KEY_SPACE ? ' ' : -1;
}
