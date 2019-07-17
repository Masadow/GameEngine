#include <allegro5/allegro.h>
#include <event/events.h>

Events::Events() : listeners() {
    queue = al_create_event_queue();
}

Events::~Events() {
    al_destroy_event_queue(queue);
}

void Events::addEvent(Event &event) {
    addEvent(event.source());
}

void Events::addEvent(ALLEGRO_EVENT_SOURCE *esrc) {
    if (!esrc)
    {
        throw EventException("Invalid event source received");
    }
    al_register_event_source(queue, esrc);
}

void Events::wait(ALLEGRO_EVENT *event) {
    al_wait_for_event(queue, event);

    auto existing_listener = listeners.find(event->type);
    if (existing_listener != listeners.end())
    {
        for (IEventListener *listener : existing_listener->second)
        {
            listener->onEvent(*event);
        }
    }
}

void Events::listen(ALLEGRO_EVENT_TYPE type, IEventListener *listener)
{
    auto existing_listener = listeners.find(type);
    if (existing_listener != listeners.end())
    {
        // Someone already listening on this event
        existing_listener->second.push_back(listener);
    }
    else
    {
        // Nobody listen to this event yet
        listeners.emplace(type, std::list<IEventListener *>({listener}));
    }
}

bool Events::isQueueEmpty() const
{
    return al_event_queue_is_empty(queue);
}