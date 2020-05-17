#ifndef CONTEXT_H
#define CONTEXT_H
#include <string>
#include <vector>
#include "Entity.h"

class Context {
public:
    std::string level;
    
    //8x8 matrix who contains the levelelements
    std::vector<std::vector<Entity*>> levelmatrix_ = std::vector<std::vector<Entity*>> (8,std::vector<Entity*>(8));

    /////These flags keep track of some states the game can be in, they are mostly used to skip large parts of the code when false
    /////this is done for optimalisation

    //Gets set to true when the currentmissile hits and get removed, leads to "loading" a new missile in the catapult and updating the missilequeue
    bool LoadNextMissile = false;
    //Gets set to true when a levelelement gets hit, leads to code that updates there status and removes them when necessary
    bool TargetsHit = false;
    //Gets set to true when levelements get removed, leads to a levelUpdate in the LevelSystem
    bool NeedLevelUpdate = false;
    
    int targetcounter;   // number of targets that aren't hit yet, when 0 the level is over

    //basic timer, increases by 1 every update
    int timer = 0;

    /////Replay information
    std::string replayFile;
    std::vector<std::string> missiles;
    std::vector<std::string> actions;
    int seed;

    bool replay = false;
    int missileCounter = 0;

};

#endif