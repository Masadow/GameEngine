#include <event/timer_event.h>

TimerEvent::TimerEvent(const double seconds, const bool autostart) {
    timer = al_create_timer(seconds);
    init_event(al_get_timer_event_source(timer));
    if (autostart) {
        start();
    }
}

TimerEvent::~TimerEvent() {
    al_destroy_timer(timer);
}

void TimerEvent::start() {
    al_start_timer(timer);
}
