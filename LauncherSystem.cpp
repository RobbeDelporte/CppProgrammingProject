#include "LauncherSystem.h"

#include <iostream>
#include <sstream>


void LauncherSystem::Update(){
    EntityStream es = engine_->GetEntityStream();   
    std::set<Entity*> entities;
    entities = es.WithTag(Component::MISSILEQUEUE);

    if(engine_->GetContext().LoadNextMissile){  
        UpdateQueue(entities);
    }

    Point mouseInput = engine_->mouseinput;

    Engine::KEY_PRESSED keyInput = engine_->keyInput;

    for(Entity* entity:entities){

        Component* component = entity->GetComponent(Component::MISSILEQUEUE);
        MissileQueueComponent* mqc = dynamic_cast<MissileQueueComponent*>(component);
        if(mqc->queuenumber==0){
            Component* component = entity->GetComponent(Component::POSITION);
            PositionComponent* pc= dynamic_cast<PositionComponent*>(component);
            //NON REPLAY CASE
            if(engine_->GetContext().replay == false){

                engine_->GetContext().missiles.push_back(std::to_string(mouseInput.x_) += std::string(" ") += std::to_string(mouseInput.y_));
                if(MissileSelected(entity,mouseInput,keyInput)){
                    engine_->GetContext().missiles.push_back(std::string("START"));
                    mqc->selected = true;
                }
                else if(mqc->selected == true && keyInput == Engine::KEY_MOUSE_UP){
                    mqc->selected = false;
                    engine_->GetContext().missiles.push_back(std::string("END"));
                    engine_->GetContext().missiles.push_back(std::to_string(mouseInput.x_) += std::string(" ") += std::to_string(mouseInput.y_));
                    LaunchMissile(entity,mqc,mouseInput);
                }
                else if(mqc->selected == true){
                    pc->position = (ConvertMouse(mouseInput)+Point(120,230))/2;           
                }
            }
            //REPLAY CASE
            else{
                if(engine_->GetContext().missiles.size() == 0){
                    std::cerr << "Replay failed: likely because of a desync between the replay and the original game. The highscorefile might be invalid" << std::endl;
                    exit(1);
                }
                std::string s = engine_->GetContext().missiles[0];
                engine_->GetContext().missiles.erase(engine_->GetContext().missiles.begin());
                if(s == std::string("START")){
                    mqc->selected = true;
                }
                else if(s == std::string("END")){
                    std::string s = engine_->GetContext().missiles[0];
                    engine_->GetContext().missiles.erase(engine_->GetContext().missiles.begin());
                    mqc->selected = false;
                    std::stringstream ss;;
                    ss << s;
                    double x;
                    double y;
                    ss >> x;
                    ss >> y;
                    mouseInput.x_ = x;
                    mouseInput.y_ = y;
                    LaunchMissile(entity,mqc,mouseInput);
                }
                else if(mqc->selected == true){
                    std::stringstream ss;;
                    ss << s;
                    double x;
                    double y;
                    ss >> x;
                    ss >> y;
                    mouseInput.x_ = x;
                    mouseInput.y_ = y;
                    //std::cout << x << " " << y << std::endl;
                    pc->position = (ConvertMouse(mouseInput)+Point(120,230))/2;
                }
            }
        }
    }
}
Point LauncherSystem::ConvertMouse(Point p){
    //Mouseinput naar standaard coord + spriteoffset
    return Point(p.x_-(MISSILE_DST_HEIGHT/2),SCREEN_HEIGHT-(p.y_-(MISSILE_DST_HEIGHT/2)));
}

bool LauncherSystem::MissileSelected(Entity* entity,Point mouseInput,Engine::KEY_PRESSED keyInput){
    PositionComponent* pc= dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
    //Cirkel hitbox, checking if there is a mouseclick the hitbox of missile 1
    if((ConvertMouse(mouseInput))*(pc->position)<=(MISSILE_DST_HEIGHT/2) && keyInput == Engine::KEY_MOUSE_DOWN && entity->HasComponent(Component::MISSILE1)){
        return true;
    }
    //rectangle(no square) hitbox, checking if there is a mouseclick the hitbox of missile 2
    else if(abs((ConvertMouse(mouseInput)).x_-(pc->position).x_)<=(MISSILE_DST_WIDTH/2) && (((0<=((ConvertMouse(mouseInput)).y_)-((pc->position).y_)) && (((ConvertMouse(mouseInput)).y_)-((pc->position).y_)<=MISSILE_DST_HEIGHT/2-8))||(((-MISSILE_DST_HEIGHT/2+4)<=((ConvertMouse(mouseInput).y_)-(pc->position).y_))&&(((ConvertMouse(mouseInput).y_)-(pc->position).y_)<=0))) && keyInput == Engine::KEY_MOUSE_DOWN && entity->HasComponent(Component::MISSILE2)){
        return true; 
        
    }
    //triangle hitbox,  checking if there is a mouseclick in the hitbox of missile 3
    

    else if(abs((ConvertMouse(mouseInput)).x_-(pc->position).x_)<=abs((ConvertMouse(mouseInput)).y_-(pc->position).y_-MISSILE_DST_HEIGHT/2)/2 && abs((ConvertMouse(mouseInput)).y_-(pc->position).y_)<=(MISSILE_DST_HEIGHT/2) && keyInput == Engine::KEY_MOUSE_DOWN && entity->HasComponent(Component::MISSILE3) ){
        return true;
    }
    else{
        return false;
    }
}

void LauncherSystem::LaunchMissile(Entity* entity,MissileQueueComponent* mqc,Point mousepos){
    ak_->PlayLaunchSound();
    engine_->RemoveEntity(entity);
    entity->Remove(mqc);
    
    double vx = std::min(LAUNCH_STRENGTH*(140 - ConvertMouse(mousepos).x_),1200.0);
    double vy = std::min(LAUNCH_STRENGTH*(220 - ConvertMouse(mousepos).y_),1200.0);
    std::cout << vx <<"  "<< vy <<std::endl;

    entity->Add(new CurrentMissileComponent(vx,vy));
    engine_->AddEntity(entity);
}

void LauncherSystem::UpdateQueue(std::set<Entity*> entities){
    for(Entity* entity:entities){
        MissileQueueComponent* mqc = dynamic_cast<MissileQueueComponent*>(entity->GetComponent(Component::MISSILEQUEUE));
        PositionComponent* mc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        mqc->queuenumber -= 1;
            if(mqc->queuenumber==0){
                mc->position = Point(140,220);
            }
            else{
                mc->position = Point(80-40*(mqc->queuenumber-1),125);
            }

    }
    Entity* me = new Entity;
    me->Add(new PositionComponent(Point(0,125)));
    int r = rand()%3;
    if(r == 0){
        me->Add(new Missile1Component());
    }
    else if(r == 1){
        me->Add(new Missile2Component());
    }
    else if(r == 2){
        me->Add(new Missile3Component());
    }
    me->Add(new MissileQueueComponent());
    engine_->AddEntity(me);
    engine_->GetContext().LoadNextMissile = false;
}