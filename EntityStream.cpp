#include "EntityStream.h"

EntityStream::EntityStream(){
    for(Component::Tag tag = (Component::Tag)0; tag !=Component::NumComponents;tag=(Component::Tag)(tag+1)){
        std::set<Entity*> entities = {};
        map.insert(std::pair<Component::Tag,std::set<Entity*>>(tag,entities));
    }
}
//returns entities in the entitystream who have a certain tag
std::set<Entity*> EntityStream::WithTag(Component::Tag tag) {
    return map.at(tag);
    
}
//returns entities in the entitystream who have all the given tags
std::set<Entity*> EntityStream::WithTagsAND(std::set<Component::Tag>& tags) {
    std::set<Component::Tag>::iterator it = tags.begin();
    std::set<Entity*> entities = map.at(*it);
    for(std::set<Component::Tag>::iterator it = tags.begin();it!=tags.end(); ++it){
        for (Entity* entity: entities){
            if (not(entity->HasComponent(*it))){
                entities.erase(entity);
            }
        }

    }
    return entities;
    
}
//returns entities in the entitystream who have one (or more) of the given tags
std::set<Entity*> EntityStream::WithTagsOR(std::set<Component::Tag>& tags) {
    std::set<Entity*> entities;
    for(Component::Tag tag: tags){
        entities.insert(map.at(tag).begin(),map.at(tag).end());
    }
    return entities;
}
//update the entitystream, remove or add a entity to the entitystream with his tags
void EntityStream::EntityUpdated(Entity* entity, std::vector<Component::Tag>& tags, bool remove) {
    if(not(remove)){
        for(Component::Tag tag: tags){
            std::set<Entity*> temp = map.at(tag);
            temp.insert(entity);
            map.at(tag) = temp;
        }
    }
    else{        
        for(Component::Tag tag: tags){
            std::set<Entity*> temp = map.at(tag);
            temp.erase(entity);
            map.at(tag) = temp;
        }
    }
}

