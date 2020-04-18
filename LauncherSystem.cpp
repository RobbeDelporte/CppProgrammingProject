#include "LauncherSystem.h"

#include <iostream>


void LauncherSystem::Update(){
    EntityStream es = GetEngine()->GetEntityStream();
    std::set<Entity*> entities;
    entities = es.WithTag(Component::MISSILE1);
    
    if(ak_->IsSpaceBarPushed()){
        std::cout << ak_->GetMouse().x_ << "    " << ak_->GetMouse().y_ << std::endl;
    }
    for(Entity* entity:entities){

        Component* component = entity->GetComponent(Component::MISSILE1);
        Missile1Component* m1c = dynamic_cast<Missile1Component*>(component);

        if(m1c->queuenumber == 0 && Missile1Selected(m1c) && m1c->selected == false){
            std::cout << "missile selected" << std::endl;
            m1c->selected = true;
        }
        else if(m1c->selected == true){
            if(ak_->IsMouseReleased()){
                std::cout << "Missile Unselected" << std::endl;
                m1c->position = ak_->GetMouse();
                m1c->selected = false;
            }
            else{
                std::cout << "Missile moved" << std::endl;
                m1c->position = ak_->GetMouse();
            }
        }
    }
}

bool LauncherSystem::Missile1Selected(Missile1Component* m1c){
    if((ak_->GetMouse())*(m1c->position)<=100.5 && ak_->IsMouseClicked()){
        return true;
    }
    else{
        return false;
    }
}