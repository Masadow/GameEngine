#ifndef __ENGINE__NETWORK__STEAM__STEAM_ROOM__
#define __ENGINE__NETWORK__STEAM__STEAM_ROOM__

#include <list>
#include "steam_api.h"
#include "../room.h"

class SteamInterface;

class SteamRoom : public Room {
    friend SteamInterface;

public:
    struct SteamMember : public Room::Member {
        CSteamID _id;

        SteamMember(CSteamID id, const std::string &name, bool self) : Room::Member(name, self), _id(id) {}
    };
private:

    bool _api_ready;
    CSteamID _id;
    std::string _name;

    std::list<std::unique_ptr<SteamMember> > _members;

    std::map<std::string, std::string> _pendingMetadata;

    void onLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure);
    CCallResult<SteamRoom, LobbyCreated_t> _lobbyCreatedCallResult;

    void onLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure);
    CCallResult<SteamRoom, LobbyEnter_t> _lobbyEnteredCallResult;

    STEAM_CALLBACK_MANUAL(SteamRoom, onLobbyDataUpdate, LobbyDataUpdate_t, _lobbyDataUpdateCallback);
    STEAM_CALLBACK_MANUAL(SteamRoom, onLobbyChatUpdate, LobbyChatUpdate_t, _lobbyChatUpdateCallback);
    STEAM_CALLBACK_MANUAL(SteamRoom, onLobbyChatMessage, LobbyChatMsg_t, _lobbyChatMessageCallback);

    void create(int slots) override;
    void join() override;

    void _resend_metadata();
    void _update_members();

public:
    SteamRoom(CSteamID * = nullptr);
    ~SteamRoom();

    void update(const double) override;

    void leave() override;

    void sendMessage(const Message &) override;

    void setMetadata(const std::string &, const std::string &) override;
    std::string getMetadata(const std::string &) override;

    void toggleCurrentMemberReady() override;

    void setName(const std::string &) override;
    const std::string &getName() const override;
    std::list<Member *> getMembers() const override;
};

#endif
