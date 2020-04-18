#ifndef MISSILE1COMPONENT_H
#define MISSILE1COMPONENT_H

#include "Component.h"
#include "Point.h"

class Missile1Component: public Component{
public:
    Missile1Component(Point p): position(p){}

    Tag GetTag(){return MISSILE1;}

    Point position;

    int queuenumber = 0;

    bool selected = false;
};
#endif