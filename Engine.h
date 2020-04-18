#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "EntityStream.h"
#include "Context.h"
#include "System.h"

class System;

class Engine
{
public:
    Engine(Context& context) {EntityStream(entitystream_);}

    void AddEntity(Entity* entity);

    void UpdateEntity(Entity* entity, std::vector<Component::Tag>& tags, bool remove);

    std::vector<Entity*>::iterator RemoveEntity(Entity* entity);

    void AddSystem(System* system);

    std::vector<System*>::iterator RemoveSystem(System* system);
    
    std::vector<Entity*>& GetEntities();

    EntityStream& GetEntityStream();

    Context& GetContext();

    void Update();
private:
    std::vector<Entity*> entities;

    std::vector<System*> systems;

    EntityStream entitystream_;

    Context context_;  
};

#endif