#ifndef __ENGINE__NETWORK__DISCORD_INTERFACE__
#define __ENGINE__NETWORK__DISCORD_INTERFACE__

#include "../network_interface.h"
#include <discord_rpc.h>

class DiscordInterface : public NetworkInterface
{
public:
    struct Options {
        const char *app_id;
    };

private:
    static DiscordInterface *_instance;
    Options _options;
    DiscordEventHandlers _handlers;
    DiscordRichPresence _presence;
    double _elapsed_since_incr;

    //handlers
    static void handleReady(const DiscordUser* request);
    static void handleDisconnected(int errorCode, const char* message);
    static void handleError(int errorCode, const char* message);
    static void handleJoin(const char* joinSecret);
    static void handleSpectate(const char* spectateSecret);
    static void handleJoinRequest(const DiscordUser* request);

public:
    DiscordInterface(Options &options);
    ~DiscordInterface();

    void update(const double) override;

    Room *createRoom(int slots) override;

//    std::list<> getFriends();
};

#endif
