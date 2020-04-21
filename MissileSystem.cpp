#include "MissileSystem.h"

void MissileSystem::Update(){
    EntityStream es = GetEngine()->GetEntityStream();
    std::set<Entity*> entities = es.WithTag(Component::CURRENTMISSILE);
    for(Entity* entity:entities){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        Component* component = entity->GetComponent(Component::POSITION);
        PositionComponent* pc= dynamic_cast<PositionComponent*>(component);
        pc->position = UpdatePosition(cmc,pc->position);
        UpdateSpeed(cmc);
    }
}

Point MissileSystem::UpdatePosition(CurrentMissileComponent* cmc, Point position){
    position.x_ += cmc->xVelocity/VELOCITYPRESCALER;
    position.y_ += cmc->yVelocity/VELOCITYPRESCALER;

    if(position.x_ < 0){
        position.x_ = 0;
    }
    else if(position.x_ > SCREEN_WIDTH - MISSILE_DST_WIDTH){ //900 - 35
        position.x_ = SCREEN_WIDTH - MISSILE_DST_WIDTH;
    }

    if(position.y_ < 90 + MISSILE_DST_HEIGHT){ //90 + 35
        position.y_ = 90 + MISSILE_DST_HEIGHT;
    }
    
    return position;
}

void MissileSystem::UpdateSpeed(CurrentMissileComponent* cmc){
    cmc->yVelocity -= GRAVITYCONSTANT;
    cmc->xVelocity /= AIRFRICTIONCONSTANT;
}