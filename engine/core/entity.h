#ifndef __ENGINE__CORE__ENTITY__
#define __ENGINE__CORE__ENTITY__

#include <map>

class Camera;

class EntityException : public std::exception
{
    std::string message;
public:
    EntityException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

class IDrawable
{
public:
    virtual ~IDrawable() {};

    virtual void draw(const Camera *) const = 0;
};

class IUpdateable
{
public:
    virtual ~IUpdateable() {};

    virtual void update(const double) = 0;
};

class IEntity : public IDrawable, public IUpdateable
{
public:
    ~IEntity() {};
};

#define ENTITY_BUILDER_DECL(type, typeId) static EntityBuilderTpl<type, typeId> _eb_; int getTypeId() const override { return typeId; }
#define ENTITY_BUILDER(type, typeId) EntityBuilderTpl<type, typeId> type::_eb_

typedef unsigned long long NetworkID;
enum PacketType : unsigned short;

class EntityBuilder;

class ANetworkEntity : public IUpdateable
{
public:
    static const int CHUNK_SIZE;

    struct PacketHeader {
        PacketType type;
        NetworkID net_id;
        NetworkID creator_id;
        int entity_type;
        bool final_chunk;
    };

private:
    static std::map<std::pair<NetworkID, NetworkID>, ANetworkEntity *> _registered;
    static NetworkID _next_net_id;
    NetworkID _net_id;
    NetworkID _creator_id;
    bool _isRegistered;
    void *_chunk;
    int _chunk_size;

protected:
    ANetworkEntity();
    ANetworkEntity(NetworkID creator_id);
    ~ANetworkEntity();

    virtual int getTypeId() const = 0;

    void *buildPacket(void *, int &, bool = true, bool = true);
    virtual void *serialize(int &) = 0;
    virtual void unserialize(void *, int) = 0;
public:

    void update(const double) override;
    virtual void onSync(void *, int);
    virtual void sync();

    inline NetworkID getNetId() const { return _net_id; };
    inline NetworkID getCreatorId() const { return _creator_id; };

    static ANetworkEntity *build(int typeId);

    static inline ANetworkEntity *getEntity(NetworkID id, NetworkID creator_id) { return _registered.find(std::make_pair(id, creator_id)) != _registered.end() ? _registered[std::make_pair(id, creator_id)] : nullptr; }
};

class EntityBuilder {
public:
    static std::map<int, EntityBuilder *> _builders;

    virtual ANetworkEntity *build() = 0;
};

template<class Type, int typeId> 
struct EntityBuilderTpl : public EntityBuilder {
    EntityBuilderTpl() {
        EntityBuilder::_builders[typeId] = this;
    }

    ANetworkEntity *build() {
        return new Type;
    }
};

#endif
