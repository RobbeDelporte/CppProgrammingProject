#ifndef MISSILEQUEUECOMPONENT_H
#define MISSILEQUEUECOMPONENT_H

#include "Component.h"
#include "Point.h"

class MissileQueueComponent: public Component{
public:
    MissileQueueComponent(){queuenumber = 3; selected = false;}

    MissileQueueComponent(int q){queuenumber = q; selected = false;}

    virtual ~MissileQueueComponent(){};

    Tag GetTag(){return MISSILEQUEUE;}

    int queuenumber;

    bool selected;
};
#endif