#include <allegro5/allegro_primitives.h>
#include "ui/widget/list.h"
#include "core/game.h"
#include "core/helper.h"
#include "ui/ui_state.h"

using namespace UI;

List::List(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(parent->getOwner(), xml, style, parent)
{
    tinyxml2::XMLElement *cur_node = xml->FirstChildElement();

    while (cur_node)
    {
        _listWidget.push_back(_build(cur_node, style));
        cur_node = cur_node->NextSiblingElement();
    }

    item_height = parse_relative_value(xml->Attribute("ih"), height);

    if (!xml->Attribute("bind")) {
        throw UIException("Missing binding for list widget");
    }
    _binding = xml->Attribute("bind");
}

List::~List()
{
    
}

void List::draw(const Camera *camera) const
{
    int cur_y = 0;

    Widget::draw(camera);

    ListBinding *listData = static_cast<ListBinding *>(getOwner()->getBinding(_binding));
    for (auto &item : listData->data)
    {
        for (auto &widget : _listWidget)
        {
            ItemBinding ib(item);

            widget->moveTo(widget->relativeX(), widget->relativeY() + cur_y);
            widget->inject(&ib);
            widget->draw(camera);
            widget->moveTo(widget->relativeX(), widget->relativeY() - cur_y);
        }
        cur_y += item_height;
    }
}

void List::update(const double elapsed)
{
    Widget::update(elapsed);
}

std::unique_ptr<Widget> List::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new List(xml, parent, style));
}