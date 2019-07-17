#include "ui/widget/viewport.h"

using namespace UI;

Viewport::Viewport(UIState *owner, tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(owner, xml, style, parent)
{

}

Viewport::~Viewport()
{
    
}

void Viewport::draw(const Camera *camera) const
{
    Widget::draw(camera);
}

void Viewport::update(const double elapsed)
{
    Widget::update(elapsed);
}

std::unique_ptr<Widget> Viewport::build_from_xml(UIState *owner, tinyxml2::XMLElement *xml, Widget *, const StyleParser &style)
{
    std::unique_ptr<Widget> widget = std::unique_ptr<Widget>(new Viewport(owner, xml, nullptr, style));
    ((Viewport*)widget.get())->build_children(xml, style);
    return widget;
}