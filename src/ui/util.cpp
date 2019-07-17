#include "ui/util.h"
#include "core/game.h"
#include "core/color.h"
#include <allegro5/allegro_primitives.h>

void drawTooltip(const Rect<int> &tooltip_area, const std::string &text)
{
    const Point<int> &mouse = Game::get()->getMouse().getMouseLocation();
    if (tooltip_area.contains(mouse)) {
        // Draw a tooltip
        Font *font = Game::get()->getDisplay()->getDefaultFont();
        int w = al_get_text_width(*font, text.c_str()) + 10;
        int h = al_get_font_line_height(*font) + 10;

        al_draw_filled_rectangle(mouse.x, mouse.y, mouse.x + w, mouse.y + h, al_map_rgba(0, 0, 0, 200));
        al_draw_text(*font, Color::WHITE, mouse.x + 5, mouse.y + 5, 0, text.c_str());
    }
}

void drawProgressBar(const Rect<int> &dim, double progress, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR bgColor)
{
    al_draw_filled_rectangle(dim.x + dim.w * progress, dim.y, dim.x + dim.w, dim.y + dim.h, bgColor);
    al_draw_filled_rectangle(dim.x, dim.y, dim.x + dim.w * progress, dim.y + dim.h, fillColor);
}