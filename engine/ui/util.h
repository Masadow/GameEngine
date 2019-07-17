#ifndef __ENGINE__UI__UTIL__
#define __ENGINE__UI__UTIL__

#include <allegro5/allegro.h>
#include "../math/rect.h"

namespace UI
{
    struct Padding {
        int left;
        int right;
        int top;
        int bottom;
    };
}

void drawTooltip(const Rect<int> &, const std::string &);
void drawProgressBar(const Rect<int> &, double progress, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR bgColor);

#endif
