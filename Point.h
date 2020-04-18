#ifndef POINT_H
#define POINT_H

#include <math.h>

class Point {
    public:
        double x_, y_;

        Point() {};
        Point(double x, double y) : x_(x), y_(y) {};

        double operator*(const Point& o) {
            double a = sqrt((x_ - o.x_)*(x_ - o.x_)+(y_ - o.y_)*(y_ - o.y_));
            return a;
        }

        Point operator-(const Point& o) {
            return Point(x_ - o.x_ , y_ - o.y_);
        }

        double operator>>(const Point& o) {
            double ip = x_*o.x_ + y_*o.y_;
            return ip;
        }

        double Length() {
            double l = sqrt(x_*x_+y_*y_);
            return l;
        }

        void Normalize() {
            x_ = x_/Length();
            y_ = y_/Length();
        }
};

#endif