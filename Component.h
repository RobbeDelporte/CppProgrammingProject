#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    enum Tag {
        SPRITE,
        BOX,
        MISSILEQUEUE,
        MISSILE,
        MISSILE1,
        CURRENTMISSILE,
        NumComponents
    };
    virtual ~Component(){};

    virtual Tag GetTag() = 0;

};

#endif