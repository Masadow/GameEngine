#ifndef __ENGINE__EVENT__EVENTS__
#define __ENGINE__EVENT__EVENTS__

#include <map>
#include <list>
#include <allegro5/allegro.h>
#include "event.h"
#include "event_listener.h"

class EventException : public std::exception
{
    std::string message;
public:
    EventException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};


class Events {
    ALLEGRO_EVENT_QUEUE *queue;
    std::map<ALLEGRO_EVENT_TYPE, std::list<IEventListener *> > listeners;

public:
    Events();
    ~Events();

    void addEvent(Event &);
    void addEvent(ALLEGRO_EVENT_SOURCE *);

    void wait(ALLEGRO_EVENT *);

    void listen(ALLEGRO_EVENT_TYPE type, IEventListener *listener);

    bool isQueueEmpty() const;
};

#endif
