#include "core/asset_mgr.h"
#include "ui/def.h"

using namespace UI;

std::map<std::string, Def::Slice> Def::_slices;

void Def::load(const std::string &path)
{
    XMLDocumentPtr doc = AssetMgr::xml(path);
    tinyxml2::XMLElement *child = doc->RootElement();

    _slices.clear();
    while (child) {
        _slices.emplace(std::make_pair(child->Name(), Slice{
            /*.src = */child->Attribute("src"),
            /*.area = */Rect<unsigned int>(std::atoi(child->Attribute("x")), std::atoi(child->Attribute("y")), std::atoi(child->Attribute("w")), std::atoi(child->Attribute("h"))),
            /*.padding = */{
                /*.left = */std::atoi(child->Attribute("padding-left")),
                /*.right = */std::atoi(child->Attribute("padding-right")),
                /*.top = */std::atoi(child->Attribute("padding-top")),
                /*.bottom = */std::atoi(child->Attribute("padding-bottom"))
            },
            /*.autoscale = */child->Attribute("autoscale", "true") != nullptr
        }));
        child = child->NextSiblingElement();
    }
}

const Def::Slice *Def::get(const std::string &key) {
    const auto &it = _slices.find(key);
    return it == _slices.end() ? nullptr : &it->second;
}
