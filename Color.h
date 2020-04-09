#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    unsigned char r_ = 0, g_ = 0, b_ = 0;

    Color() {}
    Color(unsigned char r, unsigned char g, unsigned char b) : r_(r), g_(g), b_(b) {}
};

#endif