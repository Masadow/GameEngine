#include <event/network_event.h>

NetworkEvent::NetworkEvent() {
    init_event(&event.event_source, true);
}

NetworkEvent::~NetworkEvent() {

}

