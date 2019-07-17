#ifndef __ENGINE__UI__DEF__
#define __ENGINE__UI__DEF__

#include <string>
#include "../math/rect.h"
#include "util.h"

namespace UI
{
    class Def
    {
    public:
        struct Slice {
            std::string src;
            Rect<unsigned int> area;
            Padding padding;
            bool autoscale;
        };
    private:
        static std::map<std::string, Slice> _slices;

    public:
        static void load(const std::string &path);
        static const Slice *get(const std::string &);
    };
}

#endif
