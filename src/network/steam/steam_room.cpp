#include "network/steam/steam_room.h"
#include "network/network_exception.h"

#include <iostream>

SteamRoom::SteamRoom(CSteamID *id) {
    _lobbyDataUpdateCallback.Register(this, &SteamRoom::onLobbyDataUpdate);
    _lobbyChatUpdateCallback.Register(this, &SteamRoom::onLobbyChatUpdate);
    _lobbyChatMessageCallback.Register(this, &SteamRoom::onLobbyChatMessage);
    if (id) {
        _id = *id;
        _api_ready = true;
        _name = getMetadata("name");
    } else {
        _api_ready = false;
    }
    _canceled = false;
    _inside = false;
}

SteamRoom::~SteamRoom() {
    _lobbyDataUpdateCallback.Unregister();
    _lobbyChatUpdateCallback.Unregister();
    _lobbyChatMessageCallback.Unregister();
}

void SteamRoom::update(const double elapsed) {
    Room::update(elapsed);

//    SteamMatchmaking()->RequestLobbyData()
}

void SteamRoom::create(int slots)
{
    _slots = slots;
    SteamAPICall_t pending_call = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, slots);
    if (!pending_call)
    {
        throw SteamException("Failed to create lobby");
    }
    _lobbyCreatedCallResult.Set(pending_call, this, &SteamRoom::onLobbyCreated);
// Code below is unreliable
//    _lobbyEnteredCallResult.Set(pending_call, this, &SteamRoom::onLobbyEntered);
}

void SteamRoom::join()
{
    SteamAPICall_t pending_call = SteamMatchmaking()->JoinLobby(_id);
    if (!pending_call)
    {
        throw SteamException("Failed to join lobby");
    }
    _lobbyEnteredCallResult.Set(pending_call, this, &SteamRoom::onLobbyEntered);
}

void SteamRoom::leave()
{
    SteamMatchmaking()->LeaveLobby(_id);
    _api_ready = false;
    _inside = false;
}

void SteamRoom::_resend_metadata()
{
    if (!_pendingMetadata.empty()) {
        for (auto &pair : _pendingMetadata) {
            setMetadata(pair.first, pair.second);
        }
        _pendingMetadata.clear();
        _updated = true;
    }
}

void SteamRoom::setMetadata(const std::string &key, const std::string &value)
{
    if (_api_ready) {
        if (!SteamMatchmaking()->SetLobbyData(_id, key.c_str(), value.c_str())) {
            throw SteamException("Could not set the lobby name");
        }
    } else {
        _pendingMetadata.emplace(std::make_pair(key, value));
    }
}

std::string SteamRoom::getMetadata(const std::string &key)
{
    return SteamMatchmaking()->GetLobbyData(_id, key.c_str());
}

void SteamRoom::onLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure)
{
    if ( !bIOFailure )
    {
        switch (pCallback->m_eResult)
        {
            case k_EResultOK:
                _id = CSteamID(pCallback->m_ulSteamIDLobby);
                _api_ready = true;
                _isHost = true;
                _resend_metadata();
                _update_members();
                _inside = true;
                toggleCurrentMemberReady();
                break;
            case k_EResultLimitExceeded:
                throw SteamException("You have already created too many lobbies");
            case k_EResultTimeout:
            case k_EResultNoConnection:
                throw SteamException("Steam connection has timed out");
            case k_EResultAccessDenied:
                throw SteamException("You're not allowed to create a lobby.");
            case k_EResultFail:
            default:
                throw SteamException("Could not create lobby : Unknown exception");
        }
    }
    else
    {
        throw SteamException("IO Failure");
    }
}

void SteamRoom::onLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure)
{
    if ( !bIOFailure )
    {
        switch (pCallback->m_EChatRoomEnterResponse)
        {
            case k_EChatRoomEnterResponseSuccess:
                _id = CSteamID(pCallback->m_ulSteamIDLobby);
                _slots = SteamMatchmaking()->GetLobbyMemberLimit(_id);
                _api_ready = true;
                _inside = true;
                _update_members();
                break;
            case k_EChatRoomEnterResponseDoesntExist:
                //The game might have been deleted so we cancel the join
                _canceled = true;
                break;
            case k_EChatRoomEnterResponseError:
            default:
                std::cout << pCallback->m_EChatRoomEnterResponse << std::endl;
                throw SteamException("Could not join lobby : Unknown exception");
        }
    }
    else
    {
        throw SteamException("IO Failure");
    }
}

void SteamRoom::onLobbyDataUpdate(LobbyDataUpdate_t *pCallback)
{
    if (pCallback->m_ulSteamIDLobby == _id.ConvertToUint64()) {
        _update_members();
    }
}

void SteamRoom::onLobbyChatUpdate(LobbyChatUpdate_t *pCallback)
{
    if (pCallback->m_ulSteamIDLobby == _id.ConvertToUint64()) {
        CSteamID uid(pCallback->m_ulSteamIDUserChanged);
        switch (pCallback->m_rgfChatMemberStateChange)
        {
            case k_EChatMemberStateChangeEntered:
                _members.push_back(std::make_unique<SteamMember>(uid, SteamFriends()->GetFriendPersonaName(uid), SteamUser()->GetSteamID().ConvertToUint64() == uid.ConvertToUint64()));
                _updated = true;
                break ;
            case k_EChatMemberStateChangeLeft:
            case k_EChatMemberStateChangeKicked:
            case k_EChatMemberStateChangeBanned:
                for (auto &member : _members) {
                    if (member->_id == uid) {
                        _members.remove(member);
                        _updated = true;
                        break;
                    }
                }
        }
    }
}

void SteamRoom::onLobbyChatMessage(LobbyChatMsg_t *pCallback)
{
    if (pCallback->m_ulSteamIDLobby == _id.ConvertToUint64())
    {
        switch (pCallback->m_eChatEntryType)
        {
            case k_EChatEntryTypeChatMsg:
                Room::Message msg;
                SteamMatchmaking()->GetLobbyChatEntry(_id, pCallback->m_iChatID, nullptr, &msg, sizeof(msg), nullptr);
                _unreadMessages.push(msg);
                break;
        }
    }
}

void SteamRoom::_update_members()
{
    int members = SteamMatchmaking()->GetNumLobbyMembers(_id);

    _members.clear();
    for (int idx = 0; idx < members; ++idx) {
        CSteamID mid = SteamMatchmaking()->GetLobbyMemberByIndex(_id, idx);
        _members.push_back(std::make_unique<SteamMember>(mid, SteamFriends()->GetFriendPersonaName(mid), SteamUser()->GetSteamID().ConvertToUint64() == mid.ConvertToUint64()));
        const char *v = SteamMatchmaking()->GetLobbyMemberData(_id, mid, "ready");
        if (v && strlen(v) > 0) {
            _members.back()->ready = true;
        }
    }
    _updated = true;
}

void SteamRoom::setName(const std::string &name)
{
    _name = name;
    setMetadata("name", _name);
}

const std::string &SteamRoom::getName() const 
{
    return _name;
}

std::list<Room::Member *> SteamRoom::getMembers() const
{
    std::list<Room::Member *> result;
    for (auto &member : _members) {
        result.push_back(member.get());
    }
    return result;
}

void SteamRoom::sendMessage(const Room::Message &msg)
{
    if (!_api_ready) {
        throw SteamException("Can't use the lobby network to send the message");
    }
    SteamMatchmaking()->SendLobbyChatMsg(_id, &msg, sizeof(Room::Message));
}

void SteamRoom::toggleCurrentMemberReady() {
    for (auto &member : _members) {
        if (member->self) {
            member->ready = !member->ready;
            SteamMatchmaking()->SetLobbyMemberData(_id, "ready", member->ready ? "true" : "");
        }
    }
}
