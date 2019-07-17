#ifndef __ENGINE__CORE__CAMERA__
#define __ENGINE__CORE__CAMERA__

#include "entity.h"
#include "../math/rect.h"

class Camera : public IEntity
{
    static const double CAMERA_SPEED;
    Rect<double> _pos;

public:

    Camera(Rect<double> &pos);
    Camera(double x, double y, double width, double height);
    ~Camera();

    void draw(const Camera *) const override;
    void update(const double) override;

    void moveTo(double, double);
    void centerOn(double, double);
    void centerOn(const Point<double> &);

    Rect<double> bounds;

    inline Point<double> screenToWorld(const Point<int> &sp) const
    {
        return Point<double>(sp.x + _pos.x, sp.y + _pos.y);
    }

    inline int worldToScreenX(double x) const
    {
        return x - _pos.x;
    }

    inline int worldToScreenY(double y) const
    {
        return y - _pos.y;
    }

//    inline Point<int> worldToScreen() const;

    inline int x() const
    {
        return _pos.x;
    }

    inline int y() const
    {
        return _pos.y;
    }

    inline const Rect<double> &get() const
    {
        return _pos;
    }

    inline bool isOnScreen(double x, double y) const
    {
        return x > 0 && x < _pos.w
            && y > 0 && y < _pos.h;
    }

    inline bool isOnScreen(double x, double y, double x2, double y2) const
    {
        return x2 > 0 && x < _pos.w
            && y2 > 0 && y < _pos.h;
    }
};

#endif