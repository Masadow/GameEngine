#ifndef __ENGINE__EVENT__EVENT__
#define __ENGINE__EVENT__EVENT__

class Event {
    ALLEGRO_EVENT_SOURCE *event_source;
    bool is_custom;
protected:
    Event();
    void init_event(ALLEGRO_EVENT_SOURCE *, bool custom = false);
public:
    ~Event();
    ALLEGRO_EVENT_SOURCE *source();
};

#endif
