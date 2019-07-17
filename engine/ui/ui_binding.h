#ifndef __ENGINE__UI__UI_BINDING__
#define __ENGINE__UI__UI_BINDING__

#include <string>

static std::string dumb;

struct UIBinding
{
    virtual const std::string &getString(const std::string &key) const { return dumb; };
};

namespace Binding
{
    struct Bool : public UIBinding
    {
        bool value;
    };
}

#endif
