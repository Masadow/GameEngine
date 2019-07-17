#include "ui/widget/frame.h"
#include <allegro5/allegro_primitives.h>

using namespace UI;

Frame::Frame(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(parent->getOwner(), xml, style, parent)
{
    _drawBackground = false;
    if (xml->Attribute("color"))
    {
        _drawBackground = true;
        _color = _parseColor(xml->Attribute("color"));
    }
}

Frame::~Frame()
{
    
}

void Frame::draw(const Camera *camera) const
{
    if (_drawBackground)
    {
        al_draw_filled_rectangle(absoluteX(), absoluteY(), absoluteX() + width, absoluteY() + height, _color);
    }


    drawBackground("frame");
    drawSlicedBorders("frame");

    Widget::draw(camera);
}

void Frame::update(const double elapsed)
{
    Widget::update(elapsed);
}

std::unique_ptr<Widget> Frame::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    std::unique_ptr<Widget> widget = std::unique_ptr<Widget>(new Frame(xml, parent, style));
    ((Frame*)widget.get())->build_children(xml, style);
    return widget;
}
