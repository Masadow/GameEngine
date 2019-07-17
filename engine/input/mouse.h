#ifndef __ENGINE__INPUT__MOUSE__
#define __ENGINE__INPUT__MOUSE__

#include <allegro5/allegro.h>
#include "../core/entity.h"
#include "../core/display.h"
#include "../event/events.h"
#include "../math/point.h"


class Mouse : public IEntity, public IEventListener
{
    Point<int> location;

    bool redraw;
    ALLEGRO_SYSTEM_MOUSE_CURSOR current_cursor;
    ALLEGRO_SYSTEM_MOUSE_CURSOR requested_cursor;


    bool left_button_pressed;
    bool right_button_pressed;

public:
    Mouse(Events &);
    ~Mouse();

    void setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR cursor_id);

    const Point<int> &getMouseLocation() const;

    void draw(const Camera *) const override;
    void update(const double) override;

    void consumeClick();

    void onEvent(const ALLEGRO_EVENT &event) override;

    inline bool anyPressed() { return left_button_pressed || right_button_pressed; }
    inline bool leftButtonPressed() { return left_button_pressed; };
    inline bool rightButtonPressed() { return right_button_pressed; };

    static const unsigned LEFT_BUTTON;
    static const unsigned RIGHT_BUTTON;
    static const unsigned MIDDLE_BUTTON;
};

#endif