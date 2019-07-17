#include "core/font/font.h"
#include "core/asset_mgr.h"

Font::Font() : _font(nullptr), size(0)
{

}

Font::~Font()
{
    if (_font)
    {
        al_destroy_font(_font);
    }
}

Font::Font(int size) : size(size)
{
    
}

Font::Font(std::string filename, int size) : size(size)
{
    std::string path = AssetMgr::path(filename);
    _font = al_load_font(path.c_str(), size, 0);
    if (!_font)
    {
        throw FontException("Could not load font at " + path);
    }
}

Font::operator ALLEGRO_FONT *() const
{
    return _font;
}

Font::operator const ALLEGRO_FONT *() const
{
    return _font;
}
