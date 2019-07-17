#include "core/asset_mgr.h"
#include <stdexcept>
#include <fstream>

// PRIVATE
std::string AssetMgr::asset_folder = "";
AssetMgr::AssetCache<ALLEGRO_BITMAP, AssetMgr::BmpDeallocator> AssetMgr::_bmp_cache;

//PUBLIC
const std::string AssetMgr::fileContent(const std::string &filename) {
    std::ifstream ifs(path(filename));
    return std::string( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
}

XMLDocumentPtr AssetMgr::xml(const std::string &filename) {
    std::string path = AssetMgr::asset_folder + filename;
    XMLDocumentPtr doc(new tinyxml2::XMLDocument());
    if (doc->LoadFile(path.c_str()))
    {
        throw AssetMgrException(path + ": " + doc->ErrorName());
    }
    return doc;
}

const std::string AssetMgr::path(const std::string &filename)
{
    return AssetMgr::asset_folder + filename;
}

void AssetMgr::init(const std::string &asset_folder) {
    AssetMgr::asset_folder = asset_folder;
}

void AssetMgr::clean()
{
    _bmp_cache.clean();
}

ALLEGRO_BITMAP *AssetMgr::bmp(const std::string &filename)
{
    ALLEGRO_BITMAP *cached_bmp = _bmp_cache.get(filename);
    if (cached_bmp)
    {
        return cached_bmp;
    }

    ALLEGRO_BITMAP *bmp = al_load_bitmap(path(filename).c_str());
    if (!bmp)
    {
        throw AssetMgrException("Cannot load image file : " + path(filename));
    }
    return _bmp_cache.store(filename, bmp);
}