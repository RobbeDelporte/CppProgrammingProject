#include "MissileSystem.h"
#include <sstream>

void MissileSystem::Update(){
    EntityStream es = engine_->GetEntityStream();
    std::set<Entity*> entities = es.WithTag(Component::CURRENTMISSILE);
    for(Entity* entity:entities){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        PositionComponent* pc= dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        UpdateSpeed(cmc);
        //NON REPLAY CASE
        if(engine_->GetContext().replay == false){
            if(engine_->keyInput == Engine::KEY_SPACE && cmc->SpecialActivated == false){
                engine_->GetContext().actions.push_back(std::to_string(pc->position.x_) += std::string(" ") += std::to_string(pc->position.y_));
                ActivateSpecial(entity);
            }
        }
        //REPLAY CASE
        else if(engine_->GetContext().actions.size() > 0){
            std::stringstream ss;
            ss << engine_->GetContext().actions[0];
            double x;
            double y;
            ss >> x;
            ss >> y;
            std::cout << "(" << pc->position.x_ << "," << pc->position.y_ << ") ("<< x << "," << y << ") " << (pc->position * Point(x,y)) << std::endl;
            if((pc->position * Point(x,y)) <= 4){
                engine_->GetContext().actions.erase(engine_->GetContext().actions.begin());
                ActivateSpecial(entity);
            }
        }
        pc->position = UpdatePosition(cmc,pc->position,entity);
    }
}

Point MissileSystem::UpdatePosition(CurrentMissileComponent* cmc, Point position,Entity* entity){
    position.x_ += cmc->xVelocity/VELOCITYPRESCALER;
    position.y_ += cmc->yVelocity/VELOCITYPRESCALER;

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
    return position;
}

void MissileSystem::UpdateSpeed(CurrentMissileComponent* cmc){
    if(cmc->SpecialActivated == false){
        cmc->yVelocity -= GRAVITYCONSTANT;
    }
}

void MissileSystem::ActivateSpecial(Entity* entity){
    std::cout << "activating" << std::endl;
    if (entity->HasComponent(Component::MISSILE1)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = -700;
        cmc->xVelocity = -700;
        cmc->SpecialActivated = true;
    }
    else if(entity->HasComponent(Component::MISSILE2)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = 0;
        cmc->xVelocity = 1400;
        cmc->SpecialActivated = true;
    }
    else if(entity->HasComponent(Component::MISSILE3)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = -1200;
        cmc->xVelocity = 0;
        cmc->SpecialActivated = true;
    }
    
}