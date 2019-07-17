#ifndef __ENGINE__UI__STYLE_PARSER__
#define __ENGINE__UI__STYLE_PARSER__

#include <string>
#include <map>
#include <list>

namespace UI
{
    class StyleParser
    {
        struct KeyPair {
            std::string attr;
            std::string val;
        };

        const char *_css;
        int _idx;

        bool parseSelector(std::string &selector);
        bool parseAttributeValuePair(std::string &attr, std::string &val);

        std::list<KeyPair> _empty;
        std::map<const std::string, std::list<KeyPair> > _style;
    public:

        const std::list<KeyPair> &get(const std::string &key) const;

        void parse(const std::string &);
    };
}

#endif
