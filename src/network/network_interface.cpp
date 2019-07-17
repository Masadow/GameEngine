#include "network/network_interface.h"
#include "network/network_exception.h"
#include "core/game.h"
#include <iostream>

NetworkListener::NetworkListener()
{
	Game::get()->getNetworkHandler()->setListener(this);
}

NetworkListener::~NetworkListener()
{
	//When Game is destroyed, the network interface might not exist anymore so this check is mandatory to avoid a crash on exit
	NetworkInterface *ni = Game::get()->getNetworkHandler();
	if (ni) {
		ni->removeListener(this);
	}
}

void NetworkInterface::warningHook(int severity, const char *msg)
{
	std::cout << severity << ":" << msg << std::endl;
}

NetworkInterface::NetworkInterface()
{
	_currentRoom = nullptr;
}

NetworkInterface::~NetworkInterface()
{
}

void NetworkInterface::update(const double elapsed)
{
	if (_currentRoom != nullptr) {
		_currentRoom->update(elapsed);
	}
}

void NetworkInterface::addGamePeer(NetworkPlayer *peer)
{
	_gamePeers.push_back(peer);
}

void NetworkInterface::setLocalGamePeer(NetworkPlayer *peer)
{
	_localGamePeer = peer;
}

void NetworkInterface::setListener(NetworkListener *listener)
{
	_listeners.push_back(listener);
}

void NetworkInterface::removeListener(NetworkListener *listener)
{
	_listeners.remove(listener);
}
