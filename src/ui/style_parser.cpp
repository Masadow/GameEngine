#include "ui/style_parser.h"
#include <cctype>

using namespace UI;

bool StyleParser::parseSelector(std::string &selector)
{
    selector.clear();
    while (1) {
        if (!_css[_idx]) {
            return false;
        } else if (_css[_idx] == '{') {
            return true;
        }
        if (std::isalnum(_css[_idx]) || _css[_idx] == '_') {
            selector += _css[_idx];
        }
        _idx++;
    }
}

bool StyleParser::parseAttributeValuePair(std::string &attr, std::string &val)
{
    attr.clear();
    val.clear();
    while (++_idx) {
        if (!_css[_idx] || _css[_idx] == '}') {
            return false;
        }
        if (_css[_idx] == ':') {
            break;
        }
        if (std::isalnum(_css[_idx]) || _css[_idx] == '_' || _css[_idx] == '-') {
            attr += _css[_idx];
        }
    }
    while (++_idx) {
        if (!_css[_idx] || _css[_idx] == '}') {
            return false;
        }
        if (_css[_idx] == ';') {
            return true;
        }
        if (std::isalnum(_css[_idx]) || _css[_idx] == '.' || _css[_idx] == '%' || _css[_idx] == '-') {
            val += _css[_idx];
        }
    }
    return false;
}

void StyleParser::parse(const std::string &css)
{
    _css = css.c_str();
    _idx = 0;
    std::string selector;
    std::string attr;
    std::string val;
    while (parseSelector(selector)) {
        while (parseAttributeValuePair(attr, val)) {
            if (_style.find(selector) == _style.end()) {
                _style.emplace(std::make_pair(selector, std::list<KeyPair>()));
            }
            _style[selector].push_back({
                /*.attr = */attr,
                /*.val = */val
            });
        }
    }
}

const std::list<StyleParser::KeyPair> &StyleParser::get(const std::string &key) const
{
    const auto &it = _style.find(key);
    return it == _style.end() ? _empty : it->second;
}
