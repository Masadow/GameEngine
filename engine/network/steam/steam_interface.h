#ifndef __ENGINE__NETWORK__STEAM__STEAM_INTERFACE__
#define __ENGINE__NETWORK__STEAM__STEAM_INTERFACE__

#include <list>
#include <memory>
#include "steam_api.h"
#include "../network_interface.h"
#include "steam_player.h"

class SteamInterface : public NetworkInterface {
public:
    struct Options {

    };

    struct SteamSender : public Sender {
        CSteamID id;
        SteamSender(CSteamID id) : id(id) {}
    };

private:
    struct PacketBuffer {
        void *data;
        int size;
    };

    double _next_update_timer;
    static const double REFRESH_RATE;

    ListRoomCallback _onMatchListComplete;

    void onRoomListFetched(LobbyMatchList_t *pCallback, bool bIOFailure);
    CCallResult<SteamInterface, LobbyMatchList_t> _lobbyMatchListCallResult;

    STEAM_CALLBACK_MANUAL(SteamInterface, onP2PSessionRequest, P2PSessionRequest_t, _P2PSessionRequestCallback);

    std::map<NetworkPlayer *, std::list<PacketBuffer>> _packet_queue;

public:

    SteamInterface(Options &options);
    ~SteamInterface();

    void update(const double) override;

    void broadcast(void *, int, bool = true) override;

    std::string getUsername() const override;

    Room *createRoom(int slots) override;
    void joinRoom(Room *) override;
    void listRooms(ListRoomCallback) override;

    SteamPlayer *findPeerById(uint64);
};

#endif