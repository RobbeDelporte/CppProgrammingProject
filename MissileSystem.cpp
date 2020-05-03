#include "MissileSystem.h"

void MissileSystem::Update(){
    EntityStream es = engine_->GetEntityStream();
    std::set<Entity*> entities = es.WithTag(Component::CURRENTMISSILE);
    for(Entity* entity:entities){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        PositionComponent* pc= dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        UpdateSpeed(cmc);
        if(engine_->keyInput == Engine::KEY_SPACE && cmc->SpecialActivated == false){
            ActivateSpecial(entity);
        }
        pc->position = UpdatePosition(cmc,pc->position,entity);
    }
}

Point MissileSystem::UpdatePosition(CurrentMissileComponent* cmc, Point position,Entity* entity){
    position.x_ += cmc->xVelocity/VELOCITYPRESCALER;
    position.y_ += cmc->yVelocity/VELOCITYPRESCALER;

    

    if(position.x_ < 0){
        engine_->GetContext().LoadNextMissile = true;
        engine_->RemoveEntity(entity);
        delete entity;
    }

    else if(position.x_ > SCREEN_WIDTH){ //900 - 35
        engine_->GetContext().LoadNextMissile = true;
        engine_->RemoveEntity(entity);
        delete entity;
    }
    if(position.y_ < 90 + MISSILE_DST_HEIGHT){ //90 + 35
        position.y_ = 90 + MISSILE_DST_HEIGHT;

        if(entity->HasComponent(Component::MISSILE1)){
            cmc->yVelocity = cmc->yVelocity*(-0.8);
        }
        else if(entity->HasComponent(Component::MISSILE3)){
            cmc->xVelocity *= (0.8);
            if(abs(cmc->xVelocity)<0.1){
                engine_->GetContext().LoadNextMissile = true;
                engine_->RemoveEntity(entity);
                delete(entity);
            }
        }
    }
    return position;
}

void MissileSystem::UpdateSpeed(CurrentMissileComponent* cmc){
    if(cmc->SpecialActivated == false){
        cmc->yVelocity -= GRAVITYCONSTANT;
    }
}

void MissileSystem::ActivateSpecial(Entity* entity){
    std::cout << "activating" << std::endl;
    if(entity->HasComponent(Component::MISSILE2)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = 0;
        cmc->xVelocity = 1200;
        cmc->SpecialActivated = true;
    }
    else if(entity->HasComponent(Component::MISSILE3)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = -1000;
        cmc->SpecialActivated = true;
    }
}