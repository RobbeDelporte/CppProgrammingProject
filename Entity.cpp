#include "Entity.h"


Entity::~Entity(){
    for(Component* component:GetComponents()){
        delete component;
    }
}

void Entity::Add(Component* component) {
    components.push_back(component);
}

void Entity::Remove(Component* component) {
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        if((*it) == component){
            components.erase(it);
            delete (*it);
            break;
        }
    }
}

Component* Entity::GetComponent(Component::Tag tag) {
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        if((*it)->GetTag() == tag){
            return (*it);
        }
    }
    return NULL;
}
    
std::vector<Component::Tag>& Entity::GetTags() {
    tags.clear();
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        tags.push_back((*it)->GetTag());
    }
    return tags;
}

std::vector<Component*>& Entity::GetComponents() {
    return components;
}

