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
            if(engine_->keyInput == Engine::KEY_SPACE && cmc->SpecialActivated == false){ //checks if the spacebar was pressed and the special action wasn't already used
                engine_->GetContext().actions.push_back(std::to_string(engine_->GetContext().missileCounter) += std::string(" ") += std::to_string(pc->position.x_) += std::string(" ") += std::to_string(pc->position.y_));
                ActivateSpecial(entity);
            }
        }
        //REPLAY CASE
        //same as non replay case, but we use strings from the vector actions as replacement for the spacebar
        else if(engine_->GetContext().actions.size() > 0){
            std::stringstream ss;
            ss << engine_->GetContext().actions[0];
            int missileCounter;
            double x;
            double y;
            ss >> missileCounter;
            ss >> x;
            ss >> y;
            std::cout << "(" << pc->position.x_ << "," << pc->position.y_ << ") ("<< x << "," << y << ") " << (pc->position * Point(x,y)) << std::endl;
            if((pc->position * Point(x,y)) <= 4 && missileCounter == engine_->GetContext().missileCounter){
                engine_->GetContext().actions.erase(engine_->GetContext().actions.begin());
                ActivateSpecial(entity);
            }
        }
        pc->position = UpdatePosition(cmc,pc->position,entity);
    }
}

Point MissileSystem::UpdatePosition(CurrentMissileComponent* cmc, Point position,Entity* entity){
    //updates the position per iteration by using the velocities
    position.x_ += cmc->xVelocity/VELOCITYPRESCALER;
    position.y_ += cmc->yVelocity/VELOCITYPRESCALER;

    //checking if the current missile hit the ground and what to do in the different cases
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

    if(position.x_ < 0){  //checks if the missile is out of bound to the left of the screen and removes it if true
        engine_->GetContext().LoadNextMissile = true;
        engine_->RemoveEntity(entity);
        delete entity;
    }

    else if(position.x_ > SCREEN_WIDTH){ //900 - 35, checks if the missile is out of bound to the right of the screen and removes it if true
        engine_->GetContext().LoadNextMissile = true;
        engine_->RemoveEntity(entity);
        delete entity;
    }
    return position;
}

void MissileSystem::UpdateSpeed(CurrentMissileComponent* cmc){
    //updates the y-velocity(only whe the special action is not already activated)
    if(cmc->SpecialActivated == false){
        cmc->yVelocity -= GRAVITYCONSTANT;
    }
}

void MissileSystem::ActivateSpecial(Entity* entity){
    std::cout << "activating" << std::endl;
    //activates the special action of missile1 and sets the SpecialActivated to true
    //a motion to the left and down with constant velocity
    if (entity->HasComponent(Component::MISSILE1)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = -700;
        cmc->xVelocity = -700;
        cmc->SpecialActivated = true;
    }
    //activates the special action of missile2 and sets the SpecialActivated to true
    //an acceleration to the right, very high x velocity and no more y velocity
    else if(entity->HasComponent(Component::MISSILE2)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = 0;
        cmc->xVelocity = 1400;
        cmc->SpecialActivated = true;
    }
    //activates the special action of missile3 and sets the SpecialActivated to true
    //an acceleration to the bottom, very high negative y velocity and no more x velocity 
    else if(entity->HasComponent(Component::MISSILE3)){
        CurrentMissileComponent* cmc = dynamic_cast<CurrentMissileComponent*>(entity->GetComponent(Component::CURRENTMISSILE));
        cmc->yVelocity = -1200;
        cmc->xVelocity = 0;
        cmc->SpecialActivated = true;
    }
    
}