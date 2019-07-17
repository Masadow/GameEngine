#include "input/mouse.h"
#include "core/game.h"

const unsigned Mouse::LEFT_BUTTON = 1;
const unsigned Mouse::RIGHT_BUTTON = 2;
const unsigned Mouse::MIDDLE_BUTTON = 4;

Mouse::Mouse(Events &event_manager) : location(0, 0), redraw(false), current_cursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT), left_button_pressed(false), right_button_pressed(false)
{
    al_install_mouse();

    event_manager.addEvent(al_get_mouse_event_source());
    event_manager.listen(ALLEGRO_EVENT_MOUSE_AXES, this);
    event_manager.listen(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, this);
}

Mouse::~Mouse()
{

}
    
void Mouse::setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR cursor_id)
{
    requested_cursor = cursor_id;
}

const Point<int> &Mouse::getMouseLocation() const
{
    return location;
}

void Mouse::draw(const Camera *) const
{
    if (redraw)
    {
        al_set_system_mouse_cursor(Game::get()->getDisplay()->getAllegroDisplay(), current_cursor);
    }
}

void Mouse::update(const double)
{
    consumeClick();
    redraw = current_cursor != requested_cursor;
    if (redraw)
    {
        current_cursor = requested_cursor;
    }
}

void Mouse::onEvent(const ALLEGRO_EVENT &event)
{
    if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        //Mouse moved
        location.x = event.mouse.x;
        location.y = event.mouse.y;
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.mouse.button == LEFT_BUTTON)
        {
            left_button_pressed = true;
        }
        else if (event.mouse.button == RIGHT_BUTTON)
        {
            right_button_pressed = true;
        }
    }
}

void Mouse::consumeClick()
{
    left_button_pressed = false;
    right_button_pressed = false;
}