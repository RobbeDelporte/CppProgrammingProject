#ifndef EXPLOSIONEFFECTCOMPONENT_H
#define EXPLOSIONEFFECTCOMPONENT_H

#include "Component.h"

class ExplosionEffectComponent: public Component{
public:
    ExplosionEffectComponent(Point p): position(p){};

    virtual ~ExplosionEffectComponent(){}

    Tag GetTag(){return EXPLOSIONEFFECT;}

    Point position;

    std::vector<Sprite> ExplosionAnim = {SPRT_EXPLOSION1,SPRT_EXPLOSION2,SPRT_EXPLOSION3,SPRT_EXPLOSION4,SPRT_EXPLOSION5};

    int animStage = 0;

    int animTiming = 0;

};
#endif