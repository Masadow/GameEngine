#ifndef __ENGINE__NETWORK__ROOM__
#define __ENGINE__NETWORK__ROOM__

#include <queue>
#include <list>
#include <memory>
//#include <raknet/slikenet/types.h>
//#include "steam_api.h"
#include "../core/entity.h"

class Room : public IUpdateable {
public:
    struct Member {
        std::string name;
        bool self;
        bool ready;

        Member(const std::string &name, bool self) : name(name), self(self), ready(false) {}
    };

    struct Message {
        enum MessageType {
            MT_NONE = 0, //Internal, means no messages had been sent
            MT_START = 1
        };

        MessageType type;
        char value[256];
    };

protected:

    std::queue<Message> _unreadMessages;

    virtual void create(int slots) = 0;
    virtual void join() = 0;

    bool _updated;
    bool _isHost;

    unsigned short _slots;

    bool _canceled;
    bool _inside;

public:
    Room();
    ~Room();

    virtual void setName(const std::string &) = 0;
    virtual const std::string &getName() const = 0;
    virtual std::list<Member *> getMembers() const = 0;
    void getNextMessage(Message &);

    virtual void leave() = 0;

    virtual void sendMessage(const Message &) = 0;

    virtual void setMetadata(const std::string &, const std::string &) = 0;
    virtual std::string getMetadata(const std::string &) = 0;

    virtual void toggleCurrentMemberReady() = 0;

    void update(const double) override;

    virtual inline bool hasUpdated() const { return _updated; }
    virtual inline bool isHost() const { return _isHost; }
    virtual inline unsigned short maxSlots() { return _slots; }

    inline bool isCanceled() const { return _canceled; }
    inline bool isInside() const { return _inside; }
};

#endif
