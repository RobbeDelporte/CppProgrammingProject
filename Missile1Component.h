#ifndef MISSILE1COMPONENT_H
#define MISSILE1COMPONENT_H

#include "Component.h"

class Missile1Component: public Component{
public:
    Missile1Component(){}

    virtual ~Missile1Component(){};

    Tag GetTag(){return MISSILE1;}
};
#endif