#ifndef __ENGINE__EVENT__TIMER_EVENT__
#define __ENGINE__EVENT__TIMER_EVENT__

#include <allegro5/allegro.h>
#include "event.h"

class TimerEvent : public Event {
    ALLEGRO_TIMER *timer;
public:
    TimerEvent(const double, const bool = false);
    ~TimerEvent();

    void start();
};

#endif
