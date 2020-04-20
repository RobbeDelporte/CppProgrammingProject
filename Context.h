#ifndef CONTEXT_H
#define CONTEXT_H
#include <string>
#include <vector>
#include "Entity.h"

class Context {
public:
    std::string level;
    
    std::vector<std::vector<Entity*>> levelmatrix_ = std::vector<std::vector<Entity*>> (8,std::vector<Entity*>(8));

    bool LoadNextMissile = false;

    bool TargetsHit = false;

    bool NeedLevelUpdate = false;
};

#endif