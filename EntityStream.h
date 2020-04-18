#ifndef ENTITYSTREAM_H
#define ENTITYSTREAM_H

#include <set>
#include <map>

#include "Entity.h"

class EntityStream
{
public:
    EntityStream();

    std::set<Entity*> WithTag(Component::Tag tag);

    std::set<Entity*> WithTagsAND(std::set<Component::Tag>& tags);
    
    std::set<Entity*> WithTagsOR(std::set<Component::Tag>& tags);

    void EntityUpdated(Entity* entity, std::vector<Component::Tag>& tags, bool remove);

private:
    std::map<Component::Tag,std::set<Entity*>> map;
};

#endif