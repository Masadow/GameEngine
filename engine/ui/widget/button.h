#ifndef __ENGINE__UI__WIDGET__BUTTON__
#define __ENGINE__UI__WIDGET__BUTTON__

#include <string>
#include "text.h"

class Button : public Text {
    std::string action;
public:
    Button(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~Button();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
