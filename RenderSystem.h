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

        virtual ~RenderSystem(){};
        
        void Update();

    private:
        Allkit* ak_ = NULL;
        void RenderSprite(SpriteComponent*);
        void RenderBox(PositionComponent*,LevelElementComponent*);
        void RenderStone(PositionComponent*,LevelElementComponent*);
        void RenderTarget(PositionComponent*,LevelElementComponent*,TargetComponent*);
        void RenderMissile1(PositionComponent*);
        void RenderMissile2(PositionComponent*,Missile2Component*);
        void RenderMissile3(PositionComponent*);
        bool RenderExplosion(ExplosionEffectComponent*);
};

#endif