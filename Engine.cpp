#include "Engine.h"


void Engine::AddEntity(Entity* entity) {
    UpdateEntity(entity,entity->GetTags(),false);
    entities.push_back(entity);
}

void Engine::UpdateEntity(Entity* entity, std::vector<Component::Tag>& tags, bool remove) {
    entitystream_.EntityUpdated(entity,tags,remove);
}

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

void Engine::AddSystem(System* system) {
    systems.push_back(system);
}

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

std::vector<Entity*>& Engine::GetEntities() {
    return entities;
}

void Engine::Update() {
    for(System *s : systems){
        s->Update();
    }
}

EntityStream& Engine::GetEntityStream() {
    return entitystream_;
}

Context& Engine::GetContext() {
    return context_;
}