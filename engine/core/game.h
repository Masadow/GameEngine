#ifndef __ENGINE__CORE__GAME__
#define __ENGINE__CORE__GAME__

#include <memory>
#include <list>
#include <functional>
#include "state.h"
#include "display.h"
#include "../input/mouse.h"
#include "../input/keyboard.h"
#include "../event/events.h"
#include "../network/network_interface.h"
#include "camera.h"

class Game {
    std::list<std::unique_ptr<State> > states;
    Display *display;
    const double fps;
    Camera _camera;
    Events events; // Must be kept initialized before mouse
    Mouse mouse;
    Keyboard keyboard;
    static Game *_instance;
    std::unique_ptr<NetworkInterface> _network;
    bool _is_updating;
    std::unique_ptr<State> _new_state;
    std::function<void(std::unique_ptr<State>)> _state_update;

    //FPS counter variables
    float _frameCount;
    float _totalElapsed;
    float _last_fps;

    bool _stop;

public:
    bool show_fps_counter;

    Game(Display *, const double = 60);
    ~Game();

    void run();
    void stop();

    void addState(std::unique_ptr<State>);
    void removeState(std::unique_ptr<State>);
    void switchState(std::unique_ptr<State>);

//    void setNetworkHandler(Client *);
//    void clearNetworkHandler();

    void addEvent();

    void update(const double);
    void draw() const;

    template<class NetworkClass>
    void initNetwork(typename NetworkClass::Options opt)
    {
        _network = std::make_unique<NetworkClass>(opt);
    }

    static inline Game *get() { return _instance; };

    inline Mouse &getMouse() { return mouse; };
    inline const Mouse &getMouse() const { return mouse; };
    inline Keyboard &getKeyboard() { return keyboard; };
    inline const Keyboard &getKeyboard() const { return keyboard; };
    inline const Display *getDisplay() const { return display; };
    inline Display *getDisplay() { return display; };
    inline const Camera *getCamera() const { return &_camera; };
    inline Camera *getCamera() { return &_camera; };
    inline const NetworkInterface *getNetworkHandler() const { return _network.get(); };
    inline NetworkInterface *getNetworkHandler() { return _network.get(); };

};

#endif