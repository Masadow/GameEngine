#include "core/display.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "core/asset_mgr.h"

Display::Display(int width, int height) : _cur_bmp(nullptr), width(width), height(height) {
    _display = al_create_display(width, height);
    if(!_display) {
        throw DisplayException("failed to create display!");
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
	al_init_primitives_addon();

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}

Display::~Display() {
    AssetMgr::clean();
}

ALLEGRO_DISPLAY *Display::getAllegroDisplay() const
{
    return _display;
}

ALLEGRO_EVENT_SOURCE *Display::eventSource() const {
    return al_get_display_event_source(_display);
}

void Display::setDefaultFont(Font &df)
{
    _default_font = &df;
}

Font *Display::getDefaultFont() const
{
    return _default_font;
}

void Display::startDrawing()
{
//    al_hold_bitmap_drawing(true);
    al_clear_to_color(al_map_rgb(0,0,0));
}

void Display::endDrawing() const
{
//    al_hold_bitmap_drawing(false);
    al_flip_display();
}

void Display::startBitmapDrawing(ALLEGRO_BITMAP *bmp, int flags)
{
    if (_cur_bmp)
    {
        endBitmapDrawing();
    }

    al_lock_bitmap(bmp, al_get_bitmap_format(bmp), flags);
    al_set_target_bitmap(bmp);
    _cur_bmp = bmp;
}

void Display::endBitmapDrawing()
{
    if (!_cur_bmp)
    {
        throw DisplayException("No bitmap to unlock");
    }
    al_set_target_backbuffer(_display);
    al_unlock_bitmap(_cur_bmp);
    _cur_bmp = nullptr;
}
