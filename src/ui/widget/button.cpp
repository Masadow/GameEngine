#include <allegro5/allegro.h>
#include "ui/widget/button.h"
#include "ui/ui_state.h"
#include "core/game.h"

using namespace UI;

Button::Button(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Text(xml, parent, style)
{
    consumeClick = true;
}

Button::~Button()
{
    
}

void Button::draw(const Camera *camera) const
{
    drawBackground("button", _hovered ? "hovered" : "", -width / 2);
    drawSlicedBorders("button");

    Text::draw(camera);
}

void Button::update(const double elapsed)
{
    Text::update(elapsed);

    if (visible && enabled()) {
        Rect<float> bounds = getBounds();
        Mouse &mouse = Game::get()->getMouse();
        const Point<int> &mouse_location = mouse.getMouseLocation();

        if (mouse_location.is_inside(bounds.x, bounds.y, bounds.w, bounds.h))
        {
            mouse.setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        }
    }
}

std::unique_ptr<Widget> Button::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new Button(xml, parent, style));
}