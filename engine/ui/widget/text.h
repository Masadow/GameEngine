#ifndef __ENGINE__UI__WIDGET__TEXT__
#define __ENGINE__UI__WIDGET__TEXT__

#include <string>
#include <allegro5/allegro_font.h>
#include "../widget.h"
#include "../../math/rect.h"

class Text : public Widget {
public:
    struct TextBinding : public UIBinding
    {
        std::string text;
    };

private:
    std::string text;
    ALLEGRO_COLOR color;
    int align;
    bool _isBinding;

protected:
    int _text_width;
    bool _autoWidth;

public:
    Text(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~Text();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    void _drawBorder() const override;

    Rect<float> getBounds() const override;

    void setText(const std::string &);

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
