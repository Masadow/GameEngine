#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "ui/widget/toggle.h"
#include "ui/ui_state.h"
#include "core/game.h"
#include "core/color.h"

using namespace UI;

Toggle::Toggle(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Button(xml, parent, style)
{
    if (!xml->Attribute("value"))
    {
        value = "";
    }
    else
    {
        value = xml->Attribute("value");
    }
}

Toggle::~Toggle()
{
    
}

void Toggle::draw(const Camera *camera) const
{
    if (value != "")
    {
        Binding::Bool *binding = static_cast<Binding::Bool *>(getOwner()->getBinding(value));

        if (binding->value)
        {
            Rect<float> bounds = getBounds();
            //Toggle is active
            al_draw_filled_rectangle(bounds.x, bounds.y,
                            bounds.x + bounds.w, bounds.y + bounds.h, Color::WHITE);
        }

    }

    Button::draw(camera);
}

void Toggle::update(const double elapsed)
{
    Rect<float> bounds = getBounds();
    Mouse &mouse = Game::get()->getMouse();
    const Point<int> &mouse_location = mouse.getMouseLocation();
    bool is_clicked = mouse.leftButtonPressed(); //Store the click before because the button will consume the click

    Button::update(elapsed);

    if (mouse_location.is_inside(bounds.x, bounds.y, bounds.w, bounds.h))
    {
        mouse.setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        if (is_clicked && value != "")
        {
            Binding::Bool *binding = static_cast<Binding::Bool *>(getOwner()->getBinding(value));
            binding->value = !binding->value;
            mouse.consumeClick(); // Click should already be consumed by parent but safety first
        }
    }
}

std::unique_ptr<Widget> Toggle::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new Toggle(xml, parent, style));
}