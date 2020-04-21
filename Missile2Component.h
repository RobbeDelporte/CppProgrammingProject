#ifndef MISSILE2COMPONENT_H
#define MISSILE2COMPONENT_H

#include "Component.h"

class Missile2Component: public Component{
public:
    Missile2Component(){}

    virtual ~Missile2Component(){};

    Tag GetTag(){return MISSILE2;}
};
#endif