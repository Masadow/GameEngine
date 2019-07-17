#ifndef __ENGINE__CORE__FONT__TTF_FONT__
#define __ENGINE__CORE__FONT__TTF_FONT__

#include "font.h"

class TTFFont : public Font
{
public:
    TTFFont(std::string filename, int size);
    ~TTFFont();
};

#endif