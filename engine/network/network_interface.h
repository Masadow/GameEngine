#ifndef __ENGINE__NETWORK__NETWORK_INTERFACE__
#define __ENGINE__NETWORK__NETWORK_INTERFACE__

#include <functional>
#include <list>
#include <memory>
#include "../core/entity.h"
#include "network_player.h"
#include "room.h"

enum PacketType : unsigned short {
    PT_SYNC_ENTITY = 0,
    PT_CUSTOM = 1
};

class Sender {

};

class NetworkListener {
protected:
    NetworkListener();
    virtual ~NetworkListener();
public:
    virtual void onMessage(std::unique_ptr<Sender>, void *, int) = 0;
};

class NetworkInterface : IUpdateable {
public:
    typedef std::function<void(std::list<Room *>)> ListRoomCallback;

protected:
    Room *_currentRoom;
    std::list<std::unique_ptr<Room> > _knownRooms;
    std::list<NetworkPlayer *> _gamePeers;
    NetworkPlayer *_localGamePeer;
    std::list<NetworkListener *>_listeners;

public:

    NetworkInterface();
    ~NetworkInterface();

    virtual Room *createRoom(int slots) = 0;
    virtual void joinRoom(Room *) = 0;
    virtual inline Room *getCurrentRoom() const { return _currentRoom; }

    virtual void listRooms(ListRoomCallback) = 0;

    virtual void addGamePeer(NetworkPlayer *);
    virtual void setLocalGamePeer(NetworkPlayer *);

    virtual inline const NetworkPlayer *getLocalPeer() const { return _localGamePeer; }

    void setListener(NetworkListener *);
    void removeListener(NetworkListener *);

    void update(const double) override;

    virtual void broadcast(void *, int, bool = true) = 0;

    static void warningHook(int, const char *);

    virtual std::string getUsername() const = 0;
};

#endif