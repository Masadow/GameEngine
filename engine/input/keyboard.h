#ifndef __ENGINE__INPUT__KEYBOARD__
#define __ENGINE__INPUT__KEYBOARD__

#include <exception>
#include <allegro5/allegro.h>
#include "../event/events.h"

class KeyboardException : public std::exception
{
    std::string message;
public:
    KeyboardException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

class Keyboard : public IEventListener
{
    bool pressed[ALLEGRO_KEY_MAX];
    std::list<int> just_pressed;
    std::list<int> just_released;

public:
    Keyboard(Events &);
    ~Keyboard();

    void onEvent(const ALLEGRO_EVENT &event) override;

    void reset();

    inline const std::list<int> &justPressed() const { return just_pressed; };
    inline const std::list<int> &justReleased() const { return just_released; };

    bool operator [](int keycode) const { return pressed[keycode]; };
};

#endif