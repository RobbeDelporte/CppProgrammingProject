#ifndef MISSILECOMPONENT_H
#define MISSILECOMPONENT_H

#include "Component.h"
#include "Point.h"

class MissileComponent: public Component{
public:
    MissileComponent(Point p ):position(p){}

    virtual ~MissileComponent(){};

    Tag GetTag(){return MISSILE;}

    Point position;
};
#endif