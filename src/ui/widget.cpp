#include <allegro5/allegro_primitives.h>
#include "ui/widget.h"
#include "ui/ui_state.h"
#include "core/helper.h"
#include "core/game.h"
#include "core/color.h"
#include "ui/def.h"
#include "core/asset_mgr.h"

#include "ui/widget/text.h"
#include "ui/widget/frame.h"
#include "ui/widget/input.h"
#include "ui/widget/button.h"
#include "ui/widget/list.h"
#include "ui/widget/toggle.h"
#include "ui/widget/text_list.h"

using namespace UI;

// List of supported widget built from XML
const std::map<std::string, WidgetGenerator> Widget::widget_mapping = {
    {"text", Text::build_from_xml},
    {"frame", Frame::build_from_xml},
    {"input", Input::build_from_xml},
    {"button", Button::build_from_xml},
    {"list", List::build_from_xml},
    {"toggle", Toggle::build_from_xml},
    {"text-list", TextList::build_from_xml}
};

Widget::Widget(UIState *owner, Widget *parent) : parent(parent), owner(owner), binding(nullptr), x(0), y(0), border_thick(0)
{
    _hovered = false;
    visible = true;
    width = parent ? parent->width : Game::get()->getDisplay()->width;
    height = parent ? parent->height : Game::get()->getDisplay()->height;
}

Widget::Widget(UIState *owner, tinyxml2::XMLElement *xml, const UI::StyleParser &style, Widget *parent) : Widget(owner, parent)
{
    int ref_width = parent ? parent->width : Game::get()->getDisplay()->width;
    int ref_height = parent ? parent->height : Game::get()->getDisplay()->height;

    const char *name_str = xml->Attribute("name");
    _name = name_str ? name_str : "";
    //Retrieve dimensions and position properties
    x = parse_relative_value(xml->Attribute("x"), ref_width);
    y = parse_relative_value(xml->Attribute("y"), ref_height);
    width = parse_relative_value(xml->Attribute("w"), ref_width, ref_width);
    height = parse_relative_value(xml->Attribute("h"), ref_height, ref_height);
    //Retrieve border related properties
    border_thick = xml->IntAttribute("border-thick");
    const char *border_color_str = xml->Attribute("border-color");
    border_color = border_color_str != nullptr ? hex_string_to_allegro_color(border_color_str) : al_map_rgb(255, 255, 255);
    //Retrieve conditional display attribute if exists
    const char *if_string = xml->Attribute("if");
    _conditional_key = if_string ? std::string(if_string) : "";
    consumeClick = xml->Attribute("consume-click");
    memset(&padding, 0, sizeof(padding));


    for (auto &param : style.get(_name)) {
        if (param.attr == "x") {
            x = parse_relative_value(param.val.c_str(), ref_width);
        } else if (param.attr == "y") {
            y = parse_relative_value(param.val.c_str(), ref_height);
        } else if (param.attr == "width") {
            width = parse_relative_value(param.val.c_str(), ref_width);
        } else if (param.attr == "height") {
            height = parse_relative_value(param.val.c_str(), ref_height);
        } else if (param.attr == "padding") {
            int v = parse_relative_value(param.val.c_str(), 0);
            padding = {
                /*.left = */v,
                /*.right = */v,
                /*.top = */v,
                /*.bottom = */v
            };
        } else if (param.attr == "padding-left") {
            padding.left = parse_relative_value(param.val.c_str(), 0);
        }
    }
}

Widget::~Widget()
{

}

void Widget::_drawBorder() const
{
    // Draw border
    al_draw_rectangle(absoluteX(), absoluteY(), absoluteX() + width, absoluteY() + height, border_color, border_thick);
}

void Widget::draw(const Camera *camera) const
{
    if (visible && border_thick > 0)
    {
        _drawBorder();
    }

    for (auto &child : children)
    {
        if (child->visible)
        {
            child->draw(camera);
        }
    }
}

//If repeat is true, the background is repeated to fill width / height
//If repeat is false, the background is scaled to match width / height
void Widget::drawBackground(const std::string &prefix, const std::string &suffix, int offset_x, int offset_y, int ref_width, int ref_height) const
{
    const Def::Slice *slice = nullptr;

    if (!ref_width) { ref_width = width; }
    if (!ref_height) { ref_height = height; }

    if (suffix.size()) {
        slice = Def::get(prefix + "-background-" + suffix);
    }
    if (!slice) {
        slice = Def::get(prefix + "-background");
    }

    if (slice) {
        if (slice->autoscale) {
            int x = absoluteX() + slice->padding.left + offset_x - padding.left;
            int y = absoluteY() + slice->padding.top + offset_y - padding.top;
            unsigned int w = ref_width - slice->padding.left - slice->padding.right + padding.left + padding.right;
            unsigned int h = ref_height - slice->padding.top - slice->padding.bottom + padding.top + padding.bottom;
            al_draw_scaled_bitmap(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, x, y, w, h, 0);
        } else {
            int x = absoluteX() + slice->padding.left + offset_x - padding.left;
            int width_left = ref_width - slice->padding.left - slice->padding.right + padding.left + padding.right;
            while (width_left > 0) {
                int w = std::min((unsigned int)width_left, slice->area.w);
                int y = absoluteY() + slice->padding.top + offset_y - padding.top;
                int height_left = ref_height - slice->padding.top - slice->padding.bottom + padding.top + padding.bottom;
                while (height_left > 0) {
                    int h = std::min((unsigned int)height_left, slice->area.h);
                    al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, w, h, x, y, 0);
                    height_left -= h;
                    y += h;
                }
                width_left -= w;
                x += w;
            }
        }
    }
}

void Widget::drawSlicedBorders(const std::string &prefix, bool ignorePadding) const
{
    const Def::Slice *slice;

    //Draw frame border if defined
    int top_width_to_cover = width + (ignorePadding ? 0 : padding.left + padding.right);
    int top_border_start = absoluteX() + (ignorePadding ? 0 : -padding.left);
    int bottom_width_to_cover = top_width_to_cover;
    int bottom_border_start = top_border_start;
    int left_height_to_cover = height + (ignorePadding ? 0 : padding.top + padding.bottom);
    int left_border_start = absoluteY() + (ignorePadding ? 0 : -padding.top);
    int right_height_to_cover = left_height_to_cover;
    int right_border_start = left_border_start;
    //Draw corners
    if ((slice = Def::get(prefix + "-slice-top-left"))) {
        al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, absoluteX() + (ignorePadding ? 0 : -padding.left - slice->padding.left), absoluteY() + (ignorePadding ? 0 : - padding.top - slice->padding.top), 0);
        top_width_to_cover -= slice->area.w + (ignorePadding ? 0 : -slice->padding.left);
        top_border_start += slice->area.w + (ignorePadding ? 0 : -slice->padding.left);
        left_height_to_cover -= slice->area.h + (ignorePadding ? 0 : -slice->padding.top);
        left_border_start += slice->area.h + (ignorePadding ? 0 : -slice->padding.top);
    }
    if ((slice = Def::get(prefix + "-slice-top-right"))) {
        al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, absoluteX() + width - slice->area.w + (ignorePadding ? 0 : slice->padding.right + padding.right), absoluteY() + (ignorePadding ? 0 : - padding.top - slice->padding.top), 0);
        top_width_to_cover -= slice->area.w + (ignorePadding ? 0 : -slice->padding.right);
        right_height_to_cover -= slice->area.h + (ignorePadding ? 0 : -slice->padding.top);
        right_border_start += slice->area.h + (ignorePadding ? 0 : -slice->padding.top);
    }
    if ((slice = Def::get(prefix + "-slice-bottom-left"))) {
        al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, absoluteX() + (ignorePadding ? 0 : -padding.left - slice->padding.left), absoluteY() + height - slice->area.h + (ignorePadding ? 0 : padding.bottom + slice->padding.bottom), 0);
        bottom_width_to_cover -= slice->area.w + (ignorePadding ? 0 : -slice->padding.left);
        bottom_border_start += slice->area.w + (ignorePadding ? 0 : -slice->padding.left);
        left_height_to_cover -= slice->area.h + (ignorePadding ? 0 : -slice->padding.bottom);
    }
    if ((slice = Def::get(prefix + "-slice-bottom-right"))) {
        al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, absoluteX() + width - slice->area.w + (ignorePadding ? 0 : slice->padding.right + padding.right), absoluteY() + height - slice->area.h + (ignorePadding ? 0 : padding.bottom + slice->padding.bottom), 0);
        bottom_width_to_cover -= slice->area.w + (ignorePadding ? 0 : -slice->padding.right);
        right_height_to_cover -= slice->area.h + (ignorePadding ? 0 : -slice->padding.bottom);
    }
    //Draw top border
    if ((slice = Def::get(prefix + "-slice-top"))) {
        while (top_width_to_cover > 0) {
            int w = std::min(slice->area.w, (unsigned int)top_width_to_cover);
            al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, w, slice->area.h, top_border_start, absoluteY() + (ignorePadding ? 0 : - padding.top - slice->padding.top), 0);
            top_border_start += w;
            top_width_to_cover -= w;
        }
    }
    if ((slice = Def::get(prefix + "-slice-bottom"))) {
        while (bottom_width_to_cover > 0) {
            int w = std::min(slice->area.w, (unsigned int)bottom_width_to_cover);
            al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, w, slice->area.h, bottom_border_start, absoluteY() + height - slice->area.h + (ignorePadding ? 0 : padding.bottom + slice->padding.bottom), 0);
            bottom_border_start += w;
            bottom_width_to_cover -= w;
        }
    }
    if ((slice = Def::get(prefix + "-slice-left"))) {
        while (left_height_to_cover > 0) {
            int h = std::min(slice->area.h, (unsigned int)left_height_to_cover);
            al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, h, absoluteX() + (ignorePadding ? 0 : -padding.left - slice->padding.left), left_border_start, 0);
            left_border_start += h;
            left_height_to_cover -= h;
        }
    }
    if ((slice = Def::get(prefix + "-slice-right"))) {
        while (right_height_to_cover > 0) {
            int h = std::min(slice->area.h, (unsigned int)right_height_to_cover);
            al_draw_bitmap_region(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, h, absoluteX() + width - slice->area.w + (ignorePadding ? 0 : padding.right + slice->padding.right), right_border_start, 0);
            right_border_start += h;
            right_height_to_cover -= h;
        }
    }
}

void Widget::update(const double elapsed)
{
    if (visible) {
        for (auto &child : children)
        {
            if (child->enabled())
            {
                child->update(elapsed);
            }
        }
        Mouse &mouse = Game::get()->getMouse();
        const Point<int> &mouse_location = mouse.getMouseLocation();
        Rect<float> bounds = getBounds();
        bool nv = mouse_location.is_inside(bounds.x, bounds.y, bounds.w, bounds.h);

        if (nv != _hovered) {
            if (nv) {
                _triggerEvent(MOUSE_LEAVE);
            } else {
                _triggerEvent(MOUSE_ENTER);
            }
            _hovered = nv;
        }

        if (consumeClick)
        {
            if (mouse.anyPressed() && _hovered)
            {
                mouse.consumeClick();            
                _triggerEvent(CLICK);
            }
        }
    }
}

Widget *Widget::find(const std::string &name)
{
    if (_name == name) {
        return this;
    } else {
        for (auto &widget : children) {
            Widget *result = widget->find(name);
            if (result) {
                return result;
            }
        }
    }
    return nullptr;
}


std::unique_ptr<Widget> Widget::_build(tinyxml2::XMLElement *node, const StyleParser &style)
{
    std::string name = node->Name();
    auto builder = widget_mapping.find(name);
    if (builder != widget_mapping.end())
    {
        return builder->second(node, this, style);
    }
    throw WidgetException("Invalid widget :" + name);
}

void Widget::build_children(tinyxml2::XMLElement *root, const StyleParser &style)
{
    tinyxml2::XMLElement *cur_node = root->FirstChildElement();

    while (cur_node)
    {
        std::string name = cur_node->Name();
        auto builder = widget_mapping.find(name);
        if (builder != widget_mapping.end())
        {
            children.push_back(builder->second(cur_node, this, style));
        }
        else
        {
            throw WidgetException("Invalid widget :" + name);
        }
        cur_node = cur_node->NextSiblingElement();
    }
}

void Widget::moveTo(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Widget::inject(UIBinding *b)
{
    binding = b;
}

ALLEGRO_COLOR Widget::_parseColor(const char *scolor)
{
    return hex_string_to_allegro_color(scolor ? scolor : "00000000");
}

bool Widget::enabled() const
{
    if (parent && !parent->enabled())
    {
        return false;
    }
    return _conditional_key == "" || static_cast<Binding::Bool *>(getOwner()->getBinding(_conditional_key))->value;
}

Rect<float> Widget::getBounds() const
{
    return Rect<float>(absoluteX() - padding.left, absoluteY() - padding.top, width + padding.left + padding.right, height + padding.top + padding.bottom);
}

void Widget::listen(Event e, std::function<void(void)> l)
{
    auto it = _eventListener.find(e);
    if (it == _eventListener.end()) {
        _eventListener.emplace(std::make_pair(e, std::list<std::function<void(void)> >()));
    }
    _eventListener[e].push_back(l);
}

void Widget::_triggerEvent(Event e)
{
    for (auto &f : _eventListener[e]) { f(); }
}