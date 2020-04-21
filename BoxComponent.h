#ifndef BOXCOMPONENT_H
#define BOXCOMPONENT_H

#include "Component.h"
#include "Point.h"

class BoxComponent: public Component{
public:
    BoxComponent(){};

    virtual ~BoxComponent(){};

    Tag GetTag(){return BOX;}
};

#endif