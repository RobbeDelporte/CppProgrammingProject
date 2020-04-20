#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"
#include "SpriteComponent.h"
#include "BoxComponent.h"
#include "MissileQueueComponent.h"
#include "MissileComponent.h"
#include "Missile1Component.h"
#include "CurrentMissileComponent.h"

class Entity
{
public:
    ~Entity();

    void Add(Component* component);

    void Remove(Component* component);

    Component* GetComponent(Component::Tag tag);

    std::vector<Component*>& GetComponents();

    std::vector<Component::Tag>& GetTags();

private:
    std::vector<Component*> components;
    std::vector<Component::Tag> tags;
};

#endif