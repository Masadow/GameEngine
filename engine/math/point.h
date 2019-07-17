#ifndef __ENGINE__MATH__POINT__
#define __ENGINE__MATH__POINT__

#include <cmath>
#include <iostream>

template <class Precision=int>
struct Point {
    Precision x;
    Precision y;

    Point(Precision x, Precision y) : x(x), y(y) {};
    Point() : Point(0, 0) {};
    ~Point() {};

    Point(Point<Precision> const &other)
    {
        x = other.x;
        y = other.y;
    }

    inline bool is_inside(Precision start_x, Precision start_y, Precision width, Precision height) const
    {
        return x >= start_x && x <= start_x + width
            && y >= start_y && y <= start_y + height;
    }

    void interpolate(const Point &dest, double speed)
    {
        Point<double> vector(dest.x - x, dest.y - y);

        vector.normalize();

        x += speed * vector.x;
        y += speed * vector.y;
    }

    double length() const
    {
        return std::sqrt(x * x + y * y);
    }

    void normalize()
    {
        double l = length();
        if (l > 0)
        {
            x /= l;
            y /= l;
        }
    }

    void move(Precision x_offset, Precision y_offset)
    {
        x += x_offset;
        y += y_offset;
    }

    void set(Precision n_x, Precision n_y)
    {
        x = n_x;
        y = n_y;
    }

    virtual void from(const Point<Precision> &other)
    {
        x = other.x;
        y = other.y;
    }

    double distance(const Point<Precision> &other) const
    {
        return Point<Precision>(other.x - x, other.y - y).length();
    }

    virtual bool isCloseTo(const Point<Precision> &other, double threshold) const
    {
        return distance(other) <= threshold;
    }

    Point<Precision> operator+(const Point<Precision> &other) const { return Point<Precision>(x + other.x, y + other.y); }
    Point<Precision> &operator+=(const Point<Precision> &other) { x += other.x; y += other.y; return *this; }
    Point<Precision> operator-(const Point<Precision> &other) const { return Point<Precision>(x - other.x, y - other.y); }

    bool operator==(const Point<Precision> &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point<Precision> &other) const { return x != other.x || y != other.y; }

    template <class P>
    friend std::ostream& operator<<(std::ostream& os, const Point<P> &p);
};

template <class P>
std::ostream& operator<<(std::ostream& os, const Point<P> &p) { os << "(" << p.x << "," << p.y << ")"; return os; }

#endif
