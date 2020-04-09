#ifndef POINT_H
#define POINT_H

class Point {
    public:
        double x_, y_;

        Point() {};
        Point(double x, double y) : x_(x), y_(y) {};

        double operator*(const Point& o) {
            // TODO
            return 0;
        }

        Point operator-(const Point& o) {
            // TODO
            return Point();
        }

        double operator>>(const Point& o) {
            // TODO
            return 0;
        }

        double Length() {
            // TODO
            return 0;
        }

        void Normalize() {
            // TODO
        }
};

#endif