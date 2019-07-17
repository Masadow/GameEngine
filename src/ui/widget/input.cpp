#include <allegro5/allegro_primitives.h>
#include "ui/widget/input.h"
#include "ui/ui_state.h"
#include "input/mouse.h"
#include "core/game.h"
#include "core/helper.h"

using namespace UI;

Input::Input(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(parent->getOwner(), xml, style, parent), caret(absoluteX(), absoluteY(), Game::get()->getDisplay()->getDefaultFont()->size)
{
    const char *scolor = xml->Attribute("color");
    if (!scolor)
    {
        scolor = "00000000";
    }
    color = hex_string_to_allegro_color(scolor);

    for (auto &param : style.get(_name)) {
        if (param.attr == "color") {
            color = hex_string_to_allegro_color(param.val.c_str());
        } else if (param.attr == "height" && param.val == "auto") {
            height = Game::get()->getDisplay()->getDefaultFont()->size;
        } else if (param.attr == "padding" && param.val == "auto") {
            padding = {
                /*.left = */5,
                /*.right = */5,
                /*.top = */5,
                /*.bottom = */5
            };
        }
    }

    maxLen = 0;
    editable = true;
}

Input::~Input()
{
    
}

void Input::draw(const Camera *camera) const
{
    drawBackground("input");
    drawSlicedBorders("input");

    al_draw_text(*Game::get()->getDisplay()->getDefaultFont(), color, absoluteX(), absoluteY(), ALLEGRO_ALIGN_LEFT, text.c_str());

    if (owner->getFocus() == this)
    {
        caret.draw(camera);
    }

    Widget::draw(camera);
}

void Input::update(const double elapsed)
{
    //Make sure caret color is always good
    caret.color = color;

    Mouse &mouse = Game::get()->getMouse();
    Keyboard &keyboard = Game::get()->getKeyboard();
    const Point<int> &mouse_location = mouse.getMouseLocation();

    Widget::update(elapsed);

    if (mouse_location.is_inside(absoluteX(), absoluteY(), width, height))
    {
        mouse.setCursor(editable ? ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT : ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE);
        if (editable && mouse.leftButtonPressed())
        {
            owner->setFocus(this);
            caret.reset();
        }
    }

    if (owner->getFocus() == this)
    {
        for (int keycode : keyboard.justPressed())
        {
            if (keycode == ALLEGRO_KEY_BACKSPACE) {
                if (text.size() > 0) {
                    text.pop_back();
                }
            } else if (!maxLen || text.size() < maxLen) {
                char letter = allegro_keycode_to_char(keycode);
                if (std::isprint(letter))
                {
                    text += letter;
                }
            }
        }

        caret.text_offset = al_get_text_width(*Game::get()->getDisplay()->getDefaultFont(), text.c_str());

        caret.update(elapsed);
    }
}

std::unique_ptr<Widget> Input::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new Input(xml, parent, style));
}

// CARET IMPLEMENTATION

//Caret blink time, default is same as windows default
const double Input::Caret::TOGGLE_TIMER = 0.53;
const int Input::Caret::TEXT_PADDING = 2;

Input::Caret::Caret(int x, int y, int h) : x(x), y(y), h(h), text_offset(0)
{
    reset();
}

void Input::Caret::draw(const Camera *camera) const
{
    if (displayed)
    {
        al_draw_line(x + text_offset + TEXT_PADDING, y, x + text_offset + TEXT_PADDING, y + h, color, 1);
    }
}

void Input::Caret::update(const double elapsed)
{
    timeSinceToggle += elapsed;
    if (timeSinceToggle > TOGGLE_TIMER)
    {
        timeSinceToggle = 0;
        displayed ^= true;
    }
}

void Input::Caret::reset()
{
    displayed = true;
    timeSinceToggle = 0;
}
