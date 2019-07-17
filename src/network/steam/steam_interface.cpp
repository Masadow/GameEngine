#include "network/steam/steam_interface.h"
#include "network/steam/steam_room.h"
#include "network/steam/steam_player.h"
#include "network/network_exception.h"
#include <iostream>

const double SteamInterface::REFRESH_RATE = 0.1;


SteamInterface::SteamInterface(Options &options) : _next_update_timer(0)
{
	if (!SteamAPI_Init())
	{
		throw NetworkException("Can't initialize Steam API. Make sure Steam is running.");
	}
	SteamUtils()->SetWarningMessageHook( &warningHook );
	_P2PSessionRequestCallback.Register(this, &SteamInterface::onP2PSessionRequest);
}

SteamInterface::~SteamInterface()
{
	//In the very rare scenario where packets remains to be send, properly free
	for (auto &packet_queue : _packet_queue) {
		while (!packet_queue.second.empty()) {
			auto &packet = packet_queue.second.front();
			std::free(packet.data);
			packet_queue.second.pop_front();
		}
	}

	_P2PSessionRequestCallback.Unregister();
	SteamAPI_Shutdown();
}

void SteamInterface::update(const double elapsed)
{
	//Make sure other network routines such as resetting update flags run every frames
	NetworkInterface::update(elapsed);
	//According to the steam documentation,
	//updates should have a refresh rate above 10HZ
	//to avoid lag of stacking networking packets
	_next_update_timer += elapsed;
	if (_next_update_timer > REFRESH_RATE) {
		_next_update_timer -= REFRESH_RATE;
		SteamAPI_RunCallbacks();
	}

	uint32 size;
	while (SteamNetworking()->IsP2PPacketAvailable(&size)) {
		//Should not be necessary but it feels like a '\n' is added somewhere (maybe steam API ?) so we allocate the packet of needed size + 1 to be sure it will fit
		void *packet = std::malloc( size );
		CSteamID steamIDRemote;
		uint32 bytesRead = 0;
		if ( SteamNetworking()->ReadP2PPacket( packet, size, &bytesRead, &steamIDRemote ) )
		{
			static int total = 0;
			total += size;
//			std::cout << "Receiving a packet of " << size << " bytes (total " << total << ")" << std::endl;
			ANetworkEntity::PacketHeader *header;
			ANetworkEntity *entity;
			switch (*(PacketType *)packet)
			{
				case PT_SYNC_ENTITY:
					header = (ANetworkEntity::PacketHeader *)packet;
					entity = ANetworkEntity::getEntity(header->net_id, header->creator_id);
					if (entity == nullptr) {
						entity = ANetworkEntity::build(header->entity_type);
					}
					entity->onSync(packet, size);
					break;
				case PT_CUSTOM:
					for (NetworkListener *listener : _listeners) {
						listener->onMessage(std::make_unique<SteamSender>(steamIDRemote), packet, size);
					}
					break;
			}
		}
		std::free( packet );
	}

	//Check if there are any packets pending for send
	for (auto &packet_queue : _packet_queue) {
		while (!packet_queue.second.empty()) {
			auto &packet = packet_queue.second.front();
//			std::cout << "Re-Sending a packet of " << packet.size << " bytes" << std::endl;
			if (!SteamNetworking()->SendP2PPacket(static_cast<SteamPlayer *>(packet_queue.first)->getId(), packet.data, packet.size, k_EP2PSendReliable)) {
				break ;
			}
			std::free(packet.data);
			packet_queue.second.pop_front();
		}
	}
}

void SteamInterface::broadcast(void *data, int size, bool reliable)
{
	for (NetworkPlayer *player : _gamePeers) {
		static int total = 0;
		total += size;
//		std::cout << "Sending a packet of " << size << " bytes (total " << total << ")" << std::endl;
		if (_packet_queue[player].empty()) {
			if (!SteamNetworking()->SendP2PPacket(static_cast<SteamPlayer *>(player)->getId(), data, size, reliable ? k_EP2PSendReliable : k_EP2PSendUnreliableNoDelay)) {
				if (reliable) {
					_packet_queue[player].push_back({
						/*.data = */std::malloc(size),
						/*.size = */size
					});
					memcpy(_packet_queue[player].back().data, data, size);
				}
			}
		} else if (reliable) {
			_packet_queue[player].push_back({
				/*.data = */std::malloc(size),
				/*.size = */size
			});
			memcpy(_packet_queue[player].back().data, data, size);
		}
	}
}


std::string SteamInterface::getUsername() const
{
	return SteamFriends()->GetPersonaName();
}

Room *SteamInterface::createRoom(int slots)
{
	SteamRoom *sr = new SteamRoom;
	sr->create(slots);
	_knownRooms.emplace_back(sr);
	_currentRoom = sr;
    return _currentRoom;
}

void SteamInterface::joinRoom(Room *room)
{
	SteamRoom *sr = static_cast<SteamRoom *>(room);
	sr->join();

	bool found = false;
	for (auto &uroom : _knownRooms) {
		if (uroom.get() == room) {
			_currentRoom = uroom.get();
			found = true;
		}
	}
	//Here it means it's a manually created room
	if (!found) {
		_currentRoom = sr;
	}
}

void SteamInterface::listRooms(ListRoomCallback onComplete)
{
    SteamAPICall_t call_id = SteamMatchmaking()->RequestLobbyList();
    if (!call_id)
    {
        throw SteamException("Failed to create a request to Steam API");
    }
	_onMatchListComplete = onComplete;
    _lobbyMatchListCallResult.Set(call_id, this, &SteamInterface::onRoomListFetched);
}

void SteamInterface::onRoomListFetched(LobbyMatchList_t *pCallback, bool bIOFailure)
{
	_knownRooms.clear();
	std::list<Room *> result;
	for (uint32 idx = 0; idx < pCallback->m_nLobbiesMatching; ++idx) {
		CSteamID id = SteamMatchmaking()->GetLobbyByIndex(idx);
		_knownRooms.push_back(std::make_unique<SteamRoom>(&id));
		result.push_back(_knownRooms.back().get());
	}
	_onMatchListComplete(std::move(result));
}

void SteamInterface::onP2PSessionRequest(P2PSessionRequest_t *pCallback)
{
	for (NetworkPlayer *peer: _gamePeers) {
		//Check if peer is in the same game
		if (static_cast<SteamPlayer *>(peer)->getId() == pCallback->m_steamIDRemote) {
			SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote);
		}
	}
}

SteamPlayer *SteamInterface::findPeerById(uint64 id)
{
	CSteamID steam_id(id);
	if (static_cast<SteamPlayer *>(_localGamePeer)->getId() == id) {
		return static_cast<SteamPlayer *>(_localGamePeer);
	}
	for (NetworkPlayer *peer: _gamePeers) {
		SteamPlayer *player = static_cast<SteamPlayer *>(peer);
		if (player->getId() == steam_id) {
			return player;
		}
	}
	return nullptr;
}
