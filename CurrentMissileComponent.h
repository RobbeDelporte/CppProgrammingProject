#ifndef CURRENTMISSILECOMPONENT_H
#define CURRENTMISSILECOMPONENT_H

#include "Component.h"
#include "Point.h"

class CurrentMissileComponent: public Component{
public:
    CurrentMissileComponent(double vx, double vy): xVelocity(vx), yVelocity(vy){}

    ~CurrentMissileComponent(){};

    Tag GetTag(){return CURRENTMISSILE;}

    double xVelocity;

    double yVelocity;

    bool SpecialActivated = false;
};
#endif