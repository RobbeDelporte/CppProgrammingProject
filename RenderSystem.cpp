#include "RenderSystem.h"

void RenderSystem::Update(){
    ak_->ClearScreen();
    ak_->DrawScaledBitmap(Sprite::SPRT_BACKGROUND,0,0,1200,600,0,0,900,450);

    EntityStream es = GetEngine()->GetEntityStream();
    std::set<Entity*> entities;

    entities = es.WithTag(Component::SPRITE);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::SPRITE);
        SpriteComponent* sc = dynamic_cast<SpriteComponent*>(component);
        RenderSprite(sc);
    }

    entities = es.WithTag(Component::BOX);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::BOX);
        BoxComponent* bc = dynamic_cast<BoxComponent*>(component);
        RenderBox(bc);
    }

    entities = es.WithTag(Component::MISSILE1);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::MISSILE1);
        Missile1Component* m1c = dynamic_cast<Missile1Component*>(component);
        RenderMissile1(m1c);
    }

    ak_->DrawOnScreen();
}

void RenderSystem::RenderSprite(SpriteComponent* sc){
    ak_->DrawScaledBitmap(sc->sprite,0,0,375,375,sc->location.x_,450-sc->location.y_,35,35);
}

void RenderSystem::RenderBox(BoxComponent* bc){
    int x = 600 + 35*bc->x_;
    int y = 125 + 35*bc->y_;
    ak_->DrawScaledBitmap(SPRT_BOX,0,0,375,375,x,450-y,35,35);
}

void RenderSystem::RenderMissile1(Missile1Component* m1c){
    int x = m1c->position.x_;
    int y = m1c->position.y_;
    ak_->DrawScaledBitmap(SPRT_MISSILE_1,0,0,375,375,x,450-y,35,35);
}