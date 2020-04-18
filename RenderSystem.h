#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Sprite.h"
#include "Allkit.h"

#include "Point.h"
#include "System.h"

class RenderSystem : public System {
    public:
        RenderSystem(){
            ak_ = &Allkit::Get();
        }
        
        void Update();

    private:
        Allkit* ak_ = NULL;
        void RenderSprite(SpriteComponent* sc);
        void RenderBox(BoxComponent* bc);
        void RenderMissile1(Missile1Component* mc);
};

#endif