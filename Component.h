#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    enum Tag {
        SPRITE,
        BOX,
        MISSILE1,
        NumComponents
    };
    
    virtual Tag GetTag() = 0;

};

#endif