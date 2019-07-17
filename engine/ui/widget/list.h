#ifndef __ENGINE__UI__WIDGET__LIST__
#define __ENGINE__UI__WIDGET__LIST__

#include <string>
#include <allegro5/allegro_font.h>
#include "../ui_binding.h"
#include "../widget.h"

class List : public Widget {
public:
    struct ListBinding : public UIBinding
    {
        std::list<std::map<std::string, std::string> > data;
    };
    struct ItemBinding : public UIBinding
    {
        ItemBinding(const std::map<std::string, std::string> &d) : data(d) {};
        const std::map<std::string, std::string> &data;
        virtual const std::string &getString(const std::string &key) const { if (data.find(key) == data.end()) throw WidgetException("List binding " + key + " is missing"); else return data.find(key)->second; };
    };
protected:
    int item_height;
    std::string _binding;

    std::list<std::unique_ptr<Widget> > _listWidget;

public:
    List(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~List();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
