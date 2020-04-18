#include "Entity.h"


void Entity::Add(Component* component) {
    components.push_back(component);
}

void Entity::Remove(Component* component) {
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        if((*it) == component){
            components.erase(it);
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
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        tags.push_back((*it)->GetTag());
    }
    return tags;
}

std::vector<Component*>& Entity::GetComponents() {
    return components;
}

