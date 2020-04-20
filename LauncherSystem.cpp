#include "LauncherSystem.h"

#include <iostream>


void LauncherSystem::Update(){
    EntityStream es = GetEngine()->GetEntityStream();
    std::set<Entity*> entities;
    entities = es.WithTag(Component::MISSILEQUEUE);

    if(GetEngine()->GetContext().LoadNextMissile){
        UpdateQueue(entities);
    }

    Point mouseInput = GetEngine()->mouseinput;

    Engine::KEY_PRESSED keyInput = GetEngine()->keyInput;

    for(Entity* entity:entities){

        Component* component = entity->GetComponent(Component::MISSILEQUEUE);
        MissileQueueComponent* mqc = dynamic_cast<MissileQueueComponent*>(component);
        if(mqc->queuenumber==0){
            Component* component = entity->GetComponent(Component::POSITION);
            PositionComponent* pc= dynamic_cast<PositionComponent*>(component);
            if(MissileSelected(pc,mouseInput,keyInput)){
                mqc->selected = true;
            }
            else if(mqc->selected == true && keyInput == Engine::KEY_MOUSE_UP){
                LaunchMissile(entity,mqc,mouseInput);
                mqc->selected = false;
            }
            else if(mqc->selected == true){
                pc->position = (ConvertMouse(mouseInput)+Point(120,230))/2;           
            }
        }
    }
}
Point LauncherSystem::ConvertMouse(Point p){
    //Mouseinput naar standaard coord + spriteoffset
    return Point(p.x_-(MISSILE_DST_HEIGHT/2),SCREEN_HEIGHT-p.y_+(MISSILE_DST_HEIGHT/2));
}

bool LauncherSystem::MissileSelected(PositionComponent* pc,Point mouseInput,Engine::KEY_PRESSED keyInput){
    //Cirkel hitbox
    if((ConvertMouse(mouseInput))*(pc->position)<=(MISSILE_DST_HEIGHT/2) && keyInput == Engine::KEY_MOUSE_DOWN){
        return true;
    }
    else{
        return false;
    }
}

void LauncherSystem::LaunchMissile(Entity* entity,MissileQueueComponent* mqc,Point mousepos){
    GetEngine()->RemoveEntity(entity);
    entity->Remove(mqc);
    
    double vx = std::min(LAUNCH_STRENGTH*(120 - ConvertMouse(mousepos).x_),1100.0);
    double vy = std::min(LAUNCH_STRENGTH*(230 - ConvertMouse(mousepos).y_),1000.0);
    std::cout << vx <<"  "<< vy <<std::endl;

    entity->Add(new CurrentMissileComponent(vx,vy));
    GetEngine()->AddEntity(entity);
}

void LauncherSystem::UpdateQueue(std::set<Entity*> entities){
    for(Entity* entity:entities){
        MissileQueueComponent* mqc = dynamic_cast<MissileQueueComponent*>(entity->GetComponent(Component::MISSILEQUEUE));
        PositionComponent* mc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        mqc->queuenumber -= 1;
            if(mqc->queuenumber==0){
                mc->position = Point(120,230);
            }
            else{
                mc->position = Point(80-40*(mqc->queuenumber-1),125);
            }

    }
    Entity* me = new Entity;
    me->Add(new PositionComponent(Point(0,125)));
    me->Add(new Missile1Component());
    me->Add(new MissileQueueComponent());
    GetEngine()->AddEntity(me);
    GetEngine()->GetContext().LoadNextMissile = false;
}