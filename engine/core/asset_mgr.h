#ifndef __ENGINE__CORE__ASSET_MGR__
#define __ENGINE__CORE__ASSET_MGR__

#include <exception>
#include <memory>
#include <string>
#include <tinyxml2.h>
#include <allegro5/allegro.h>
#include <map>

typedef std::unique_ptr<tinyxml2::XMLDocument> XMLDocumentPtr;

class AssetMgrException : public std::exception
{
    std::string message;
public:
    AssetMgrException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

class AssetMgr {
    static std::string asset_folder;

    template <class AllocatedType>
    struct DefaultDeallocator
    {
        static void del(AllocatedType *v)
        {
            delete v;
        }
    };

    struct BmpDeallocator : public DefaultDeallocator<ALLEGRO_BITMAP>
    {
        static void del(ALLEGRO_BITMAP *bmp)
        {
            al_destroy_bitmap(bmp);
        } 
    };

    template <class AssetType, class Deallocator = DefaultDeallocator<AssetType> >
    class AssetCache {
        std::map<std::string, AssetType *> _content;

    public:

        void clean() {
            for (auto it = _content.begin(); it != _content.end(); it++ )
            {
                Deallocator::del(it->second);
            }
        }

        AssetType *store(std::string id, AssetType *ptr)
        {
            if (!_content.emplace(id, ptr).second) {
                throw AssetMgrException("Tried to cache an asset with the same key");
            }
            return ptr;
        }

        AssetType *get(std::string id)
        {
            auto it = _content.find(id);
            return it == _content.end() ? nullptr : it->second;
        }
    };

    static AssetCache<ALLEGRO_BITMAP, BmpDeallocator> _bmp_cache;
public:

    static const std::string fileContent(const std::string &filename);
    static XMLDocumentPtr xml(const std::string &filename);
    static const std::string path(const std::string &filename);
    static ALLEGRO_BITMAP *bmp(const std::string &filename);
    
    static void init(const std::string &asset_folder);
    static void clean();
};

#endif