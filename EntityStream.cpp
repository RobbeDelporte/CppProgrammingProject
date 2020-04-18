#include "EntityStream.h"

EntityStream::EntityStream(){
    for(Component::Tag tag = (Component::Tag)0; tag !=Component::NumComponents;tag=(Component::Tag)(tag+1)){
        std::set<Entity*> entities;
        map.insert(std::pair<Component::Tag,std::set<Entity*>>(tag,entities));
    }
}

std::set<Entity*> EntityStream::WithTag(Component::Tag tag) {
    return map.at(tag);
    
}

/*std::set<Entity*> EntityStream::WithTagsAND(std::set<Component::Tag>& tags) {
    // TODO
}*/

std::set<Entity*> EntityStream::WithTagsOR(std::set<Component::Tag>& tags) {
    std::set<Entity*> entities;
    for(Component::Tag tag: tags){
        entities.insert(map.at(tag).begin(),map.at(tag).end());
    }
    return entities;
}
void EntityStream::EntityUpdated(Entity* entity, std::vector<Component::Tag>& tags, bool remove) {
    if(not(remove)){
        for(Component::Tag tag: tags){
            if(map.count(tag)==0){
                std::set<Entity*> entities;
                entities.insert(entity);
                map.insert(std::pair<Component::Tag,std::set<Entity*>>(tag,entities));
            }
            else{
                std::set<Entity*> temp = map.at(tag);
                temp.insert(entity);
                map.at(tag) = temp;
            }
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

