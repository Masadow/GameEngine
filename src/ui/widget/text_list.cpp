#include <allegro5/allegro_primitives.h>
#include "ui/widget/text_list.h"
#include "core/game.h"
#include "core/helper.h"
#include "ui/ui_state.h"
#include "core/color.h"
#include "ui/def.h"

using namespace UI;

TextList::TextList(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style) : Widget(parent->getOwner(), xml, style, parent)
{
    text_color = Color::BLACK;

    row_height = Game::get()->getDisplay()->getDefaultFont()->size;

    for (auto &param : style.get(_name)) {
        if (param.attr == "color") {
            text_color = hex_string_to_allegro_color(param.val.c_str());
        } else if (param.attr == "row_height") {
            row_height = parse_relative_value(param.val.c_str(), row_height);
        }
    }

    _hovered_idx = -1;
    _selected_idx = -1;

    selectionEnabled = true;
    unselectionEnabled = true;

    getItemColor = nullptr;
}

TextList::~TextList()
{
    
}

void TextList::draw(const Camera *camera) const
{
    if (visible) {
        Font *font = Game::get()->getDisplay()->getDefaultFont();

        const Def::Slice *slice = Def::get("text-list-item-background");
        const Def::Slice *slice_hovered = Def::get("text-list-item-background-hovered");
        const Def::Slice *slice_selected = Def::get("text-list-item-background-selected");
//        const Def::Slice *border_top = Def::get("text-list-slice-top");

        int y_offset = slice->padding.top;

        int idx = 0;
        for (auto &item : _content)
        {
            const Def::Slice *drawn_slice = slice;

            if (slice_hovered && _hovered_idx == idx) {
                drawn_slice = slice_hovered;
            } else if (slice_selected && _selected_idx == idx) {
                drawn_slice = slice_selected;
            }

            if (drawn_slice) {
                al_draw_scaled_bitmap(AssetMgr::bmp(drawn_slice->src), drawn_slice->area.x, drawn_slice->area.y, drawn_slice->area.w, drawn_slice->area.h, absoluteX() + drawn_slice->padding.left, absoluteY() + y_offset, width - drawn_slice->padding.left - drawn_slice->padding.right, row_height, 0);
            }

            al_draw_text(*font, getItemColor ? getItemColor(idx) : text_color, absoluteX() + padding.left, absoluteY() + y_offset + (row_height - al_get_font_line_height(*font)) / 2, 0, item.first.c_str());
            y_offset += row_height;
            idx++;
        }

        while (slice && y_offset < height - slice->padding.bottom) {
            int h = std::min((height - slice->padding.bottom) - y_offset, row_height);
            if (slice) {
                al_draw_scaled_bitmap(AssetMgr::bmp(slice->src), slice->area.x, slice->area.y, slice->area.w, slice->area.h, absoluteX() + slice->padding.left, absoluteY() + y_offset, width - slice->padding.left - slice->padding.right, h, 0);
            }
            y_offset += h;
        }
    }

    drawSlicedBorders("text-list", true);

    Widget::draw(camera);
}

void TextList::update(const double elapsed)
{
    //Because parent might consume the click for us, make sure we check if a click has occured
    Mouse &mouse = Game::get()->getMouse();
    bool mouse_pressed = mouse.anyPressed();
    Widget::update(elapsed);

    if (selectionEnabled && visible) {
        const Point<int> &mouse_location = mouse.getMouseLocation();
        const Def::Slice *slice = Def::get("text-list-item-background");
        int y_offset = slice->padding.top;
        int idx = 0;
        _hovered_idx = -1;

        while (idx < (int)_content.size()) {
            bool nv = mouse_location.is_inside(
                absoluteX(),
                absoluteY() + y_offset,
                width,
                row_height
            );
            if (nv) {
                mouse.setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                _hovered_idx = idx;
            }
            if (nv != _hovered) {
                if (nv) {
                    _triggerEvent(MOUSE_LEAVE_ITEM);
                } else {
                    _triggerEvent(MOUSE_ENTER_ITEM);
                }
                _hovered = nv;
            }

            if (mouse_pressed && _hovered_idx == idx)
            {
                _selected_idx = unselectionEnabled && _selected_idx == idx ? -1 : idx;
                mouse.consumeClick();
                _triggerEvent(CLICK_ITEM);
            }

            idx++;
            y_offset += row_height;
        }
    }
}

void TextList::addItem(const std::string &item, const std::string &custom_id)
{
    _content.push_back(std::make_pair(item, custom_id));
}

void TextList::clear()
{
    clearSelection();
    _content.clear();
}

void TextList::clearSelection()
{
    _selected_idx = -1;
}

std::unique_ptr<Widget> TextList::build_from_xml(tinyxml2::XMLElement *xml, Widget *parent, const StyleParser &style)
{
    return std::unique_ptr<Widget>(new TextList(xml, parent, style));
}