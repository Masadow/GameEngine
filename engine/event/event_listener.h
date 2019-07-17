#ifndef __ENGINE__EVENT__EVENT_LISTENER__
#define __ENGINE__EVENT__EVENT_LISTENER__

#include <allegro5/allegro.h>

class IEventListener {
public:
    IEventListener() {};
    ~IEventListener() {};

    virtual void onEvent(const ALLEGRO_EVENT &event) = 0;
};

#endif
