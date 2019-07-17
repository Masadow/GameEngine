#ifndef __ENGINE__MATH__RECT__
#define __ENGINE__MATH__RECT__

#include "point.h"

template <class Precision = int>
struct Rect : public Point<Precision> {
    using Point<Precision>::x;
    using Point<Precision>::y;
    Precision w;
    Precision h;

    Rect() : Rect(0, 0, 0, 0) {};
    Rect(Precision x, Precision y, Precision w, Precision h) : Point<Precision>(x, y), w(w), h(h) {};
    Rect(const Point<Precision> &p) : Rect(p.x, p.y, 0, 0) {};
    ~Rect() {};

    Rect(Rect<Precision> const &other)
    {
        x = other.x;
        y = other.y;
        w = other.w;
        h = other.h;
    }

    inline Precision centerX() const { return x + w / 2.;}
    inline Precision centerY() const { return y + h / 2.;}

    void set(int n_x, int n_y, int n_w, int n_h)
    {
        Point<Precision>::set(n_x, n_y);
        w = n_w;
        h = n_h;
    }

    virtual bool isCloseTo(const Point<Precision> &other, double threshold) const
    {
        Point<Precision> center(x + w / 2, y + h / 2);
        return center.isCloseTo(other, threshold + (w + h) / 4 );
    }

    bool contains(const Point<Precision> &point) const
    {
        return x <= point.x && x + w >= point.x && y <= point.y && y + h >= point.y;
    }

    bool overlaps(const Rect<Precision> &other) const
    {
        return ((x < other.x + other.w && x >= other.x) || (other.x < x + w && other.x >= x))
            && ((y < other.y + other.h && y >= other.y) || (other.y < y + h && other.y >= y));
    }

    Point<Precision> center() const
    {
        return Point<Precision>(x + w / 2, y + h / 2);
    }

    template <class P>
    friend std::ostream& operator<<(std::ostream& os, const Rect<P> &r);

};

template <class P>
std::ostream& operator<<(std::ostream& os, const Rect<P> &r) { os << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")"; return os; }

#endif
