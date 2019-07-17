#ifndef __ENGINE__UI__WIDGET__INPUT__
#define __ENGINE__UI__WIDGET__INPUT__

#include <string>
#include "../widget.h"

class Input : public Widget {
    struct Caret : IEntity {
        static const double TOGGLE_TIMER;
        static const int TEXT_PADDING;

        int x;
        int y;
        int h;
        int text_offset;
        bool displayed;
        double timeSinceToggle;
        ALLEGRO_COLOR color;

        Caret(int, int, int);
        ~Caret() {};

        void draw(const Camera *camera) const;
        void update(const double);

        void reset();
    };

    Caret caret;
public:
    Input(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
    ~Input();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);

    ALLEGRO_COLOR color;
    std::string text;
    unsigned int maxLen;
    bool editable;
};

#endif
