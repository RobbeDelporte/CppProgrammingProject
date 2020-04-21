#ifndef MISSILE3COMPONENT_H
#define MISSILE3COMPONENT_H

#include "Component.h"

class Missile3Component: public Component{
public:
    Missile3Component(){}

    virtual ~Missile3Component(){};

    Tag GetTag(){return MISSILE3;}
};
#endif