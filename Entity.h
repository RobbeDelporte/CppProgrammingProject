#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"
#include "SpriteComponent.h"
#include "BoxComponent.h"
#include "MissileQueueComponent.h"
#include "PositionComponent.h"
#include "Missile1Component.h"
#include "Missile2Component.h"
#include "Missile3Component.h"
#include "CurrentMissileComponent.h"
#include "LevelElementComponent.h"
#include "StoneComponent.h"
#include "TargetComponent.h"

class Entity
{
public:
    ~Entity(){
        for(Component* component:GetComponents()){
            delete component;
        }
    };

    void Add(Component* component);

    void Remove(Component* component);

    Component* GetComponent(Component::Tag tag);

    std::vector<Component*>& GetComponents();

    std::vector<Component::Tag>& GetTags();

    bool HasComponent(Component::Tag);

private:
    std::vector<Component*> components;
    std::vector<Component::Tag> tags;
};

#endif