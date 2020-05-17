#include "Engine.h"

//adds an entity to the vector 'entities' ,using  a pointer to the entity as argument
//also adds the entity to the entitystream along with its tags (by calling UpdateEntity)
void Engine::AddEntity(Entity* entity) {
    UpdateEntity(entity,entity->GetTags(),false);
    entities.push_back(entity);
     
}

//UpdateEntity has the same purpose as EntityUpdated in the classs EntityStream
//adds the entity and its tags to the entitystream, when the boolean remove is false
void Engine::UpdateEntity(Entity* entity, std::vector<Component::Tag>& tags, bool remove) {
    entitystream_.EntityUpdated(entity,tags,remove);
}

//removes an entity from the vector 'entities' and 
//removes the entity from the entitystream by giving a pointer to the entity as argument
std::vector<Entity*>::iterator Engine::RemoveEntity(Entity* entity) {
    std::vector<Entity*>::iterator it;
    for(it = entities.begin(); it != entities.end(); ++it){
        if((*it) == entity){
            UpdateEntity(entity,entity->GetTags(),true);
            entities.erase(it);
            break;
        }
    }
    return it;
}

//adds a system to the vector 'systems' using a pointer to the system as argument
void Engine::AddSystem(System* system) {
    systems.push_back(system);
}

//removes a system from the vector 'systems' using a pointer to the system as argument
//also returns an iterator to the next entity
std::vector<System*>::iterator Engine::RemoveSystem(System* system) {
    std::vector<System*>::iterator it;
    for(it = systems.begin(); it != systems.end(); ++it){
        if((*it) == system){
            systems.erase(it);
            break;
        }
    }
    return it;
}

//returns a vector of Entity pointers from all the enities in the engine
std::vector<Entity*>& Engine::GetEntities() {
    return entities;
}

//returns a vector of System pointers from all the systems in the engine
std::vector<System*>& Engine::GetSystems() {
    return systems;
}

//calls the Update-function of all the systems in the engine
void Engine::Update() {
    for(System *s : systems){
        s->Update();
    }
}

//returns a reference to the current entitystream
EntityStream& Engine::GetEntityStream() {
    return entitystream_;
}

//returns a reference to the current context
Context& Engine::GetContext() {
    return context_;
}