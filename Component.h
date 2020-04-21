#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    enum Tag {
        SPRITE,
        BOX,
        STONE,
        TARGET,
        LEVELELEMENT,
        MISSILEQUEUE,
        POSITION,
        MISSILE1,
        MISSILE2,
        MISSILE3,
        CURRENTMISSILE,
        NumComponents
    };
    virtual ~Component(){};

    virtual Tag GetTag() = 0;

};

#endif