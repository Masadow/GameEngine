#ifndef __ENGINE__UI__WIDGET__TOGGLE__
#define __ENGINE__UI__WIDGET__TOGGLE__

#include <string>
#include "button.h"

class Toggle : public Button {
    std::string value;
public:
    Toggle(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~Toggle();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
