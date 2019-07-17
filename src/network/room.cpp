#include "network/room.h"
#include <cstring>

Room::Room() : _updated(false), _isHost(false)
{

}

Room::~Room()
{

}

void Room::update(const double)
{
    _updated = false;
}

void Room::getNextMessage(Message &msg)
{
    if (_unreadMessages.size() == 0) {
        msg.type = Message::MT_NONE;
    } else {
        std::memcpy(&msg, &_unreadMessages.front(), sizeof(Message));
        _unreadMessages.pop();
    }
}
