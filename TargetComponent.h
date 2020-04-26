#ifndef TARGETCOMPONENT_H
#define TARGETCOMPONENT_H

#include "Component.h"
#include "Point.h"
#include "Sprite.h"
#include "Constants.h"
#include "vector"
#include "Allkit.h"

class TargetComponent: public Component{
public:

    TargetComponent(){};

    virtual ~TargetComponent(){};

    Tag GetTag(){return TARGET;}

    std::vector<Sprite> TargetAnim = {SPRT_SLIME1,SPRT_SLIME2};

    int animStage = 0;

    int animTiming = 0;
};

#endif