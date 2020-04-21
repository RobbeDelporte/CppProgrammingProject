#ifndef LEVELELEMENTCOMPONENT_H
#define LEVELELEMENTCOMPONENT_H

#include "Component.h"
#include "Point.h"

class LevelElementComponent:public Component{
public:
    LevelElementComponent(Point mp):matrixPosition(mp){IsHit = false; HitCounter = 0;};

    virtual ~LevelElementComponent(){};

    Tag GetTag(){return LEVELELEMENT;}

    Point matrixPosition;

    bool IsHit;

    int HitCounter;
};

#endif