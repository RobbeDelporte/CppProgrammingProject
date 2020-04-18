#ifndef BOXCOMPONENT_H
#define BOXCOMPONENT_H

#include "Component.h"
#include "Point.h"

class BoxComponent: public Component{
public:
    BoxComponent(int x,int y):x_(x),y_(y){};

    Tag GetTag(){return BOX;}

    int hp;

    int x_;

    int y_;
};

#endif