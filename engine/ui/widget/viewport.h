#ifndef __ENGINE__UI__WIDGET__VIEWPORT__
#define __ENGINE__UI__WIDGET__VIEWPORT__

#include <string>
#include "../widget.h"
#include "../style_parser.h"

class Viewport : public Widget {
public:
    Viewport(UIState *, tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~Viewport();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(UIState *owner, tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
