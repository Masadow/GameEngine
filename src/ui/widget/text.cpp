#include <allegro5/allegro_primitives.h>
#include "ui/widget/text.h"
#include "core/game.h"
#include "core/helper.h"
#include "ui/ui_state.h"

using namespace UI;

Text::Text(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(parent->getOwner(), xml, style, parent)
{
    _autoWidth = false;
    const char *scolor = xml->Attribute("color");
    if (!scolor)
    {
        scolor = "00000000";
    }
    color = hex_string_to_allegro_color(scolor);
    if (xml->Attribute("bind"))
    {
        text = xml->Attribute("bind");
        _isBinding = true;
    }
    else
    {
        const char *txt = xml->GetText();
        text = txt ? txt : "";
        _isBinding = false;
    }
    _text_width = al_get_text_width(*Game::get()->getDisplay()->getDefaultFont(), text.c_str());

    align = ALLEGRO_ALIGN_LEFT;
    if (xml->Attribute("align", "center"))
    {
        align = ALLEGRO_ALIGN_CENTER;
    }
    else if (xml->Attribute("align", "right"))
    {
        align = ALLEGRO_ALIGN_RIGHT;
    }

    for (auto &param : style.get(_name)) {
        if (param.attr == "align") {
            if (param.val == "center") {
                align = ALLEGRO_ALIGN_CENTER;
            } else if (param.val == "right") {
                align = ALLEGRO_ALIGN_RIGHT;
            }
        } else if (param.attr == "color") {
            color = hex_string_to_allegro_color(param.val.c_str());
        } else if (param.attr == "width" && param.val == "auto") {
            _autoWidth = true;
            width = _text_width;
        } else if (param.attr == "height" && param.val == "auto") {
            height = Game::get()->getDisplay()->getDefaultFont()->size;
        }
    }
}

Text::~Text()
{
    
}

Rect<float> Text::getBounds() const
{
    if (align == ALLEGRO_ALIGN_CENTER)
    {
        return Rect<float>(absoluteX() - (width >> 1) - padding.left,
                    absoluteY() - padding.top,
                    width + padding.left + padding.right,
                    Game::get()->getDisplay()->getDefaultFont()->size + padding.top + padding.bottom);
    }
    else
    {
        return Rect<float>(absoluteX() - padding.left,
                    absoluteY() - padding.top,
                    width + padding.left + padding.right,
                    Game::get()->getDisplay()->getDefaultFont()->size + padding.top + padding.bottom);
    }
}

void Text::_drawBorder() const
{
    Rect<float> bounds = getBounds();

    al_draw_rectangle(bounds.x,
                    bounds.y,
                    bounds.x + bounds.w,
                    bounds.y + bounds.h,
                    border_color, border_thick);
}

void Text::draw(const Camera *camera) const
{
    if (visible) {
        const std::string *text_to_display = &text;
        if (_isBinding)
        {
            if (binding)
            {
                //If data has been injected
                text_to_display = &binding->getString(text);
            }
            else if (!getOwner()->hasBinding(text))
            {
                throw WidgetException("Expected a binding " + text);
            }
            else
            {
                //Otherwise, lookup for global bindings
                text_to_display = &static_cast<TextBinding *>(getOwner()->getBinding(text))->text;
            }
        }
        al_draw_text(*Game::get()->getDisplay()->getDefaultFont(), color, absoluteX(), absoluteY(), align, text_to_display->c_str());
    }
    Widget::draw(camera);
}

void Text::update(const double elapsed)
{
    Widget::update(elapsed);
}

std::unique_ptr<Widget> Text::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new Text(xml, parent, style));
}

void Text::setText(const std::string &new_text)
{
    text = new_text;
    _isBinding = false;
    _text_width = al_get_text_width(*Game::get()->getDisplay()->getDefaultFont(), text.c_str());
    if (_autoWidth) {
        width = _text_width;
    }
}
