#ifndef BOXCOMPONENT_H
#define BOXCOMPONENT_H

#include "Component.h"
#include "Point.h"

class BoxComponent: public Component{
public:
    BoxComponent(){HitCounter = 0;BoxHit = false;};

    virtual ~BoxComponent(){};

    Tag GetTag(){return BOX;}

    bool BoxHit;

    int HitCounter;
};

#endif