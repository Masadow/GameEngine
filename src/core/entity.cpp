#include <cstring>
#include "core/entity.h"
#include "core/game.h"
#include "network/network_interface.h"

std::map<std::pair<NetworkID, NetworkID>, ANetworkEntity *> ANetworkEntity::_registered;
NetworkID ANetworkEntity::_next_net_id = 1; // Starts from 1 so users can use 0 for null id
std::map<int, EntityBuilder *> EntityBuilder::_builders;
const int ANetworkEntity::CHUNK_SIZE = 4096;


ANetworkEntity::ANetworkEntity() : _isRegistered(false)
{
    _chunk = nullptr;
    _chunk_size = 0;
}

ANetworkEntity::ANetworkEntity(NetworkID creator_id) : _creator_id(creator_id)
{
    _net_id = _next_net_id++;
    _registered.insert(std::make_pair(std::make_pair(_net_id, creator_id), this));
    _isRegistered = true;
    _chunk = nullptr;
    _chunk_size = 0;
}

ANetworkEntity::~ANetworkEntity()
{
    _registered.erase(std::make_pair(_net_id, _creator_id));
}

void *ANetworkEntity::buildPacket(void *data, int &size, bool allowRealloc, bool finalChunk)
{
    void *packet;
    if (allowRealloc) {
        packet = std::realloc(data, size + sizeof(PacketHeader));
    } else {
        packet = std::malloc(size + sizeof(PacketHeader));
    }

//    PacketHeader header{
//        /*.type = */PT_SYNC_ENTITY,
//        /*.net_id = */_net_id,
//        /*.creator_id = */_creator_id,
//        /*.entity_type = */getTypeId(),
//        /*.final_chunk = */finalChunk
//    };
    

    std::memmove((char *)packet + sizeof(PacketHeader), allowRealloc ? packet : data, size);
    *(PacketHeader *)packet = {
        /*.type = */PT_SYNC_ENTITY,
        /*.net_id = */_net_id,
        /*.creator_id = */_creator_id,
        /*.entity_type = */getTypeId(),
        /*.final_chunk = */finalChunk
    };
//    std::memcpy(packet, &header, sizeof(header));

    size += sizeof(PacketHeader);
    return packet;
}

void ANetworkEntity::sync()
{
    void *data;
    int size;
    data = serialize(size);
    int sentCount = 0;
    while (sentCount < size) {
        int chunkSize = std::min(size - sentCount, CHUNK_SIZE);
        int rest_to_send = size - sentCount - chunkSize;
        void *packet = buildPacket((char *)data + sentCount, chunkSize, false, rest_to_send == 0);
        Game::get()->getNetworkHandler()->broadcast(packet, chunkSize);
        std::free(packet);
        sentCount += CHUNK_SIZE;
    }
    std::free(data);
}

void ANetworkEntity::onSync(void *packet, int size)
{
    PacketHeader *ph = static_cast<PacketHeader *>(packet);
//    std::cout << "Syncing chunk of " << ph->net_id << ph->creator_id << std::endl;
    if (!_isRegistered) {
        _net_id = ph->net_id;
        _creator_id = ph->creator_id;
        _registered.insert(std::make_pair(std::make_pair(_net_id, _creator_id), this));
    }
    if (!ph->final_chunk || _chunk) {
        _chunk_size += size - sizeof(PacketHeader);
        _chunk = std::realloc(_chunk, _chunk_size);
        std::memcpy((char *)_chunk + _chunk_size - (size - sizeof(PacketHeader)), (char *)packet + sizeof(PacketHeader), size - sizeof(PacketHeader));
    }
    if (ph->final_chunk) {
        if (_chunk) {
            std::cout << "Final chunk " << _chunk_size << std::endl;
            unserialize(_chunk, _chunk_size);
            std::free(_chunk);
            _chunk = nullptr;
            _chunk_size = 0;
        } else {
            unserialize((char *)packet + sizeof(PacketHeader), size - sizeof(PacketHeader));
        }
    }
}

void ANetworkEntity::update(const double)
{

}

ANetworkEntity *ANetworkEntity::build(int typeId)
{
    auto builder = EntityBuilder::_builders.find(typeId);
    if (builder != EntityBuilder::_builders.end()) {
        return builder->second->build();
    } else {
        throw EntityException("Could not build entity from type id " + typeId);
    }
}
