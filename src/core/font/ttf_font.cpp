#include <allegro5/allegro_ttf.h>
#include "core/font/ttf_font.h"
#include "core/asset_mgr.h"

TTFFont::~TTFFont()
{

}

TTFFont::TTFFont(std::string filename, int size) : Font(size)
{
    std::string path = AssetMgr::path(filename);
    _font = al_load_ttf_font(path.c_str(), size, 0);
    if (!_font)
    {
        throw FontException("Could not load TTF font at " + path);
    }
}
