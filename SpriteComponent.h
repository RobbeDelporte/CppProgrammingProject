#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Component.h"
#include "Sprite.h"
#include "Point.h"

class SpriteComponent: public Component{
public:
    SpriteComponent(Sprite s, Point p): sprite(s), location(p){};

    Tag GetTag(){return SPRITE;}

    Sprite sprite;

    Point location;
};

#endif