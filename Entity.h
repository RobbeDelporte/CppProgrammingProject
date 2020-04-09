#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"

class Entity
{
public:
    void Add(Component* component);

    void Remove(Component* component);

    Component* GetComponent(Component::Tag tag);

    std::vector<Component*>& GetComponents();

    std::vector<Component::Tag>& GetTags();
};

#endif