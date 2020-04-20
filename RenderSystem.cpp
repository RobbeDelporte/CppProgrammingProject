#include "RenderSystem.h"

void RenderSystem::Update(){
    ak_->ClearScreen();
    ak_->DrawScaledBitmap(Sprite::SPRT_BACKGROUND,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

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
        BoxComponent* bc = dynamic_cast<BoxComponent*>(entity->GetComponent(Component::BOX));

        PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        RenderBox(pc,bc);
    }

    entities = es.WithTag(Component::MISSILE1);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::POSITION);
        PositionComponent* pc = dynamic_cast<PositionComponent*>(component);
        RenderMissile1(pc);
    }

    ak_->DrawOnScreen();
}

void RenderSystem::RenderSprite(SpriteComponent* sc){
    ak_->DrawScaledBitmap(sc->sprite,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,sc->position.x_,SCREEN_HEIGHT-sc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

void RenderSystem::RenderBox(PositionComponent* pc,BoxComponent* bc){
    Sprite s;
    bc->BoxHit ? s=SPRT_BOX_HIT : s = SPRT_BOX; 
    
    ak_->DrawScaledBitmap(s,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

void RenderSystem::RenderMissile1(PositionComponent* pc){
    ak_->DrawScaledBitmap(SPRT_MISSILE_1,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}