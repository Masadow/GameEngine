#ifndef __ENGINE__UI__WIDGET__AREA__
#define __ENGINE__UI__WIDGET__AREA__

#include <string>
#include "../widget.h"

class Frame : public Widget {
private:
    bool _drawBackground;
    ALLEGRO_COLOR _color;
public:
    Frame(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~Frame();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
