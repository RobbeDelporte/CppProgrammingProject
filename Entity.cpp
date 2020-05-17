#include "Entity.h"


//adds a component to a vector 'components' by using a pointer to the component as argument
void Entity::Add(Component* component) {
    components.push_back(component);
}
//removes a compoment from the vector components
void Entity::Remove(Component* component) {
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        if((*it) == component){
            components.erase(it);
            delete (*it);
            break;
        }
    }
}

//returns a pointer to a component from the entity by using a tag of the component
//returns NULL when the entity doesn't have a component with the given tag
Component* Entity::GetComponent(Component::Tag tag) {
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        if((*it)->GetTag() == tag){
            return (*it);
        }
    }
    return NULL;
}

//returns a vector of all the tags from the components of the entity    
std::vector<Component::Tag>& Entity::GetTags() {
    tags.clear();
    for(std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
        tags.push_back((*it)->GetTag());
    }
    return tags;
}

//returns a vector of Component pointers to all the components of the entity
std::vector<Component*>& Entity::GetComponents() {
    return components;
}

//checks if the entity has a component with a given tag
bool Entity::HasComponent(Component::Tag tag){
    for(Component* component: components){
        if(component->GetTag() == tag){
            return true;
        }
    }
    return false;
}