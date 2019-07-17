#include <allegro5/allegro.h>
#include "core/camera.h"
#include "core/game.h"

const double Camera::CAMERA_SPEED = 2048;

Camera::Camera(Rect<double> &pos) : _pos(pos)
{

}

Camera::Camera(double x, double y, double width, double height) : _pos(x, y, width, height)
{

}

Camera::~Camera()
{

}

void Camera::draw(const Camera *) const
{

}

void Camera::update(const double elapsed)
{
    if (Game::get()->getKeyboard()[ALLEGRO_KEY_LEFT] || Game::get()->getKeyboard()[ALLEGRO_KEY_A])
    {
        moveTo(_pos.x - CAMERA_SPEED * elapsed, _pos.y);
    }
    if (Game::get()->getKeyboard()[ALLEGRO_KEY_RIGHT] || Game::get()->getKeyboard()[ALLEGRO_KEY_D])
    {
        moveTo(_pos.x + CAMERA_SPEED * elapsed, _pos.y);
    }
    if (Game::get()->getKeyboard()[ALLEGRO_KEY_UP] || Game::get()->getKeyboard()[ALLEGRO_KEY_W])
    {
        moveTo(_pos.x, _pos.y - CAMERA_SPEED * elapsed);
    }
    if (Game::get()->getKeyboard()[ALLEGRO_KEY_DOWN] || Game::get()->getKeyboard()[ALLEGRO_KEY_S])
    {
        moveTo(_pos.x, _pos.y + CAMERA_SPEED * elapsed);
    }
}

void Camera::moveTo(double x, double y)
{
    _pos.x = std::max(bounds.x, std::min(bounds.x + bounds.w - _pos.w, x));
    _pos.y = std::max(bounds.y, std::min(bounds.y + bounds.h - _pos.h, y));
}

void Camera::centerOn(double x, double y)
{
    moveTo( x - Game::get()->getDisplay()->width / 2,
            y - Game::get()->getDisplay()->height / 2);
}

void Camera::centerOn(const Point<double> &p)
{
    centerOn(p.x, p.y);
}
