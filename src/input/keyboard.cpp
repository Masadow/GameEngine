#include "input/keyboard.h"
#include "core/game.h"

Keyboard::Keyboard(Events &event_manager)
{
    if (!al_install_keyboard())
    {
        throw KeyboardException("Keyboard already installed or cannot be installed.");
    }

    event_manager.addEvent(al_get_keyboard_event_source());
    event_manager.listen(ALLEGRO_EVENT_KEY_DOWN, this);
    event_manager.listen(ALLEGRO_EVENT_KEY_UP, this);

    for (int i = 0; i < ALLEGRO_KEY_MAX; ++i)
    {
        pressed[i] = false;
    }
}

Keyboard::~Keyboard()
{

}

void Keyboard::onEvent(const ALLEGRO_EVENT &event)
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        just_pressed.push_back(event.keyboard.keycode);
        pressed[event.keyboard.keycode] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        just_released.push_back(event.keyboard.keycode);
        pressed[event.keyboard.keycode] = false;
    }
}

void Keyboard::reset()
{
    just_pressed.clear();
    just_released.clear();
}