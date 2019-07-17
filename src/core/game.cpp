#include "core/game.h"
#include <allegro5/allegro.h>
#include <event/timer_event.h>
#include <iostream>
#include "core/color.h"

Game *Game::_instance = nullptr;

Game::Game(Display *display, const double fps) : display(display), fps(fps), _camera(0, 0, display->width, display->height), events(), mouse(events), keyboard(events), show_fps_counter(false)
{
    _instance = this;
    _new_state = nullptr;
    _is_updating = false;

    _frameCount = 0;
    _totalElapsed = 0;
    _last_fps = fps;

    events.addEvent(display->eventSource());
    Color::init();
}

Game::~Game() {
	_network.release();
}

void Game::addState(std::unique_ptr<State> new_state) {
    if (_is_updating) {
        _state_update = std::bind(&Game::addState, this, std::placeholders::_1);
        _new_state = std::move(new_state);
    } else {
        states.push_back(std::move(new_state));
    }
}

void Game::removeState(std::unique_ptr<State> state_to_remove) {
    if (_is_updating) {
        _state_update = std::bind(&Game::removeState, this, std::placeholders::_1);
        _new_state = std::move(state_to_remove);
    } else {
        states.remove(std::move(state_to_remove));
    }
}

void Game::switchState(std::unique_ptr<State> new_state) {
    if (_is_updating) {
        _state_update = std::bind(&Game::switchState, this, std::placeholders::_1);
        _new_state = std::move(new_state);
    } else {
        states.clear();
        addState(std::move(new_state));
    }
}

void Game::run() {
    TimerEvent te = TimerEvent(1 / fps);
    events.addEvent(te);
    ALLEGRO_EVENT event;
    double last_timer = al_get_time();
    bool redraw = false;
    _stop = false;

    te.start();
    while (!_stop) {
        events.wait(&event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break ;
        }
        if (redraw && events.isQueueEmpty())
        {
            redraw = false;
            double elapsed = al_get_time() - last_timer;
            last_timer += elapsed;
            
            update(elapsed);
            draw();
        }
    }
}

void Game::stop()
{
    _stop = true;
}

void Game::update(const double elapsed) {
    _is_updating = true;
    _totalElapsed += elapsed;

    if (++_frameCount == 60) {
        _last_fps = _frameCount / _totalElapsed;
        _frameCount = 0;
        _totalElapsed = 0;
    }

    _camera.update(elapsed);

    if (_network.get())
    {
        _network->update(elapsed);
    }

    mouse.setCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    for (auto state_it = states.rbegin(); state_it != states.rend(); ++state_it) {
        (*state_it)->update(elapsed);
    }
    mouse.update(elapsed);
    keyboard.reset();
    _is_updating = false;

    //Check if a state updated has been delayed
    if (_new_state != nullptr) {
        _state_update(std::move(_new_state));
    }
}

void Game::draw() const {
    display->startDrawing();
    for (auto &state : states) {
        state->draw(&_camera);
    }
    mouse.draw(&_camera);

    if (show_fps_counter)
    {
        char buf[32];
        memset(buf, 0, 32);
        snprintf(buf, 31, "%2.0f", _last_fps);
        al_draw_text(*display->getDefaultFont(), Color::WHITE, 5, 5, ALLEGRO_ALIGN_LEFT, buf);
    }

    //Camera is drawn last to allow for post process effects
    _camera.draw(&_camera);

    display->endDrawing();
}
