#ifndef __ENGINE__UI__WIDGET__
#define __ENGINE__UI__WIDGET__

#include <list>
#include <map>
#include <functional>
#include <tinyxml2.h>
#include <memory>
#include <string>
#include <allegro5/allegro.h>
#include "../core/entity.h"
#include "../math/rect.h"
#include "ui_binding.h"
#include "style_parser.h"
#include "util.h"

class UIState;

class WidgetException : public std::exception
{
    std::string msg;
public:
    WidgetException(std::string msg) : msg(msg) {};
    ~WidgetException() {};
    virtual const char* what() const throw() { return msg.c_str(); };
};

class Widget;

// Widger builder function prototype
typedef std::unique_ptr<Widget>(*WidgetGenerator)(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);

class Widget : public IEntity {
public:
    enum Event {
        CLICK,
        MOUSE_ENTER,
        MOUSE_LEAVE,
        MOUSE_ENTER_ITEM,
        MOUSE_LEAVE_ITEM,
        CLICK_ITEM
    };
private:
    // Map of all existing widgets, allow to build a widget from string (used while parsing .xml files)
    static const std::map<std::string, WidgetGenerator> widget_mapping;

    std::list<std::unique_ptr<Widget> > children;

    std::map<Event, std::list<std::function<void(void)> > > _eventListener;

protected:
    std::unique_ptr<Widget> _build(tinyxml2::XMLElement *, const UI::StyleParser &);
    void build_children(tinyxml2::XMLElement *, const UI::StyleParser &);

    Widget *parent;
    UIState *owner;
    UIBinding *binding;

    std::string _name;
    int x;
    int y;
    int width;
    int height;
    int border_thick;
    ALLEGRO_COLOR border_color;
    std::string _conditional_key;
    UI::Padding padding;
    bool _hovered;

    virtual void _drawBorder() const;

    ALLEGRO_COLOR _parseColor(const char *);

    void _triggerEvent(Event);

    void drawBackground(const std::string &, const std::string &suffix = "", int offset_x = 0, int offset_y = 0, int ref_width = 0, int ref_height = 0) const;
    void drawSlicedBorders(const std::string &, bool ignorePadding = false) const;

public:
    Widget(UIState *owner, Widget * = nullptr);
    Widget(UIState *owner, tinyxml2::XMLElement *xml, const UI::StyleParser &style, Widget * = nullptr);
    virtual ~Widget();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    Widget *find(const std::string &name);

    void moveTo(int, int);

    void inject(UIBinding *);

    virtual Rect<float> getBounds() const;
    inline int relativeX() const { return x; };
    inline int relativeY() const { return y; };
    inline int absoluteX() const { return parent ? x + parent->absoluteX() : x;}
    inline int absoluteY() const { return parent ? y + parent->absoluteY() : y;}
    inline UIState *getOwner() { return owner; }
    inline const UIState *getOwner() const { return owner; }
    bool enabled() const;

    bool visible;
    bool consumeClick;

    void listen(Event, std::function<void(void)>);
};

#endif
