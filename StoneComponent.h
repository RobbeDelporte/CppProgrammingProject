#ifndef STONECOMPONENT_H
#define STONECOMPONENT_H

#include "Component.h"
#include "Point.h"

class StoneComponent: public Component{
public:
    StoneComponent(){};

    virtual ~StoneComponent(){};

    Tag GetTag(){return STONE;}
};

#endif