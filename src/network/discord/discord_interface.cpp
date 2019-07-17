#include "network/discord/discord_interface.h"
#include <cstring>
#include <iostream>
#include <chrono>

DiscordInterface* DiscordInterface::_instance = nullptr;

DiscordInterface::DiscordInterface(Options &options) : _options(options), _elapsed_since_incr(0)
{
    _instance = this;

    //Initialize handlers
    memset(&_handlers, 0, sizeof(_handlers));
    _handlers.ready = handleReady;
    _handlers.disconnected = handleDisconnected;
    _handlers.errored = handleError;
    _handlers.joinGame = handleJoin;
    _handlers.spectateGame = handleSpectate;
    _handlers.joinRequest = handleJoinRequest;

    //Initialize presence
    memset(&_presence, 0, sizeof(_presence));
    _presence.state = "Playing Solo";
    _presence.details = "Competitive";
    _presence.largeImageText = "Numbani";
    _presence.smallImageText = "Rogue - Level 100";
    _presence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    _presence.partySize = 1;
    _presence.partyMax = 4;
    _presence.spectateSecret = "MTIzNDV8MTIzNDV8MTMyNDU0";
    _presence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
    _presence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    Discord_Initialize(_options.app_id, &_handlers, 1, NULL);
}

DiscordInterface::~DiscordInterface()
{
    Discord_Shutdown();
}

//Handlers
void DiscordInterface::handleReady(const DiscordUser* connectedUser)
{
    std::cout
        << "Discord: connected to user "
        << connectedUser->username
        << "#"
        << connectedUser->discriminator
        << " - "
        << connectedUser->userId
        << std::endl;
}

void DiscordInterface::handleDisconnected(int errorCode, const char* message)
{
    std::cerr << errorCode << ": " << message << std::endl;
}

void DiscordInterface::handleError(int errorCode, const char* message)
{
    std::cerr << errorCode << ": " << message << std::endl;
}

void DiscordInterface::handleJoin(const char* joinSecret)
{
    std::cout << "Join: " << joinSecret << std::endl;
}

void DiscordInterface::handleSpectate(const char* spectateSecret)
{
    std::cout << "Spectate: " << spectateSecret << std::endl;
}

void DiscordInterface::handleJoinRequest(const DiscordUser* request)
{
        std::cout
        << "Discord: join request from "
        << request->username
        << "#"
        << request->discriminator
        << " - "
        << request->userId
        << std::endl;
}

void DiscordInterface::update(const double elapsed)
{
    NetworkInterface::update(elapsed);

    Discord_UpdatePresence(&_presence);

#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();
}

Room *DiscordInterface::createRoom(int slots)
{
    return nullptr;
    /*
	SteamRoom *sr = new SteamRoom;
	sr->create(slots);
	_knownRooms.emplace_back(sr);
	_currentRoom = sr;
    return _currentRoom;
    */
}
