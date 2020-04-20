#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Component.h"
#include "Point.h"

class PositionComponent: public Component{
public:
    PositionComponent(Point p ):position(p){}

    virtual ~PositionComponent(){};

    Tag GetTag(){return POSITION;}

    Point position;
};
#endif