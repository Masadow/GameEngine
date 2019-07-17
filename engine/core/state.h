#ifndef __ENGINE__CORE__STATE__
#define __ENGINE__CORE__STATE__

#include <list>
#include "entity.h"

class State : public IEntity {
private:
    std::list<State *> substates;
public:
    bool active;

    State();
    ~State();

    virtual void draw(const Camera *) const override;
    virtual void update(const double) override;

    void addSubstate(State &);
};

#endif
