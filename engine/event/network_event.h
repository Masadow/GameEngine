#ifndef __ENGINE__EVENT__NETWORK_EVENT__
#define __ENGINE__EVENT__NETWORK_EVENT__

#include <allegro5/allegro.h>
#include "event.h"

class NetworkEvent : public Event {
    struct NetworkEventSource {
        ALLEGRO_EVENT_SOURCE event_source;
    };
    NetworkEventSource event;
public:
    NetworkEvent();
    ~NetworkEvent();
};

#endif
