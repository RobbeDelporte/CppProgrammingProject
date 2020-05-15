#ifndef MISSILE2COMPONENT_H
#define MISSILE2COMPONENT_H

#include "Component.h"

class Missile2Component: public Component{
public:
    Missile2Component(){}

    virtual ~Missile2Component(){};

    Tag GetTag(){return MISSILE2;}

    std::vector<Sprite> Missile2Anim = {SPRT_BEE1,SPRT_BEE2,SPRT_BEE3,SPRT_BEE2};

    int animStage = 0;

    int animTiming = 0;
};
#endif