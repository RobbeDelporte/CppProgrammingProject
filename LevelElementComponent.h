#ifndef LEVELELEMENTCOMPONENT_H
#define LEVELELEMENTCOMPONENT_H

#include "Component.h"
#include "Point.h"

class LevelElementComponent:public Component{
public:
    LevelElementComponent(Point mp):matrixPosition(mp){};

    virtual ~LevelElementComponent(){};

    Tag GetTag(){return LEVELELEMENT;}

    Point matrixPosition;
};

#endif