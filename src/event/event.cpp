#include <allegro5/allegro.h>
#include <event/event.h>

Event::Event() : event_source(NULL), is_custom(false) {

}

Event::~Event() {
    if (is_custom)
    {
        al_destroy_user_event_source(event_source);
    }
}

void Event::init_event(ALLEGRO_EVENT_SOURCE *src, bool custom) {
    event_source = src;
    is_custom = custom;
    if (custom)
    {
        al_init_user_event_source(event_source);
    }
}

ALLEGRO_EVENT_SOURCE *Event::source() {
    return event_source;
}