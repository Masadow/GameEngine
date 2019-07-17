#include <core/state.h>

State::State() : active(true) {

}

State::~State() {

}

void State::addSubstate(State &substate)
{
    substates.push_back(&substate);
}

void State::draw(const Camera *camera) const
{
    if (active)
    {
        for (auto state : substates)
        {
            if (state->active)
            {
                state->draw(camera);
            }
        }
    }
}

void State::update(const double elapsed)
{
    if (active)
    {
        for (auto state_it = substates.rbegin(); state_it != substates.rend(); ++state_it)
        {
            if ((*state_it)->active)
            {
                (*state_it)->update(elapsed);
            }
        }
    }
}
