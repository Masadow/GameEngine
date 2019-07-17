#ifndef __ENGINE__NETWORK__STEAM__STEAM_PLAYER__
#define __ENGINE__NETWORK__STEAM__STEAM_PLAYER__

#include "../network_player.h"
#include "steam_api.h"

class SteamPlayer : public NetworkPlayer {
    CSteamID _id;
public:
    SteamPlayer(CSteamID);
    ~SteamPlayer();

    inline CSteamID getId() const { return _id; }

    void update(const double) override;
};

#endif
