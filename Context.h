#ifndef CONTEXT_H
#define CONTEXT_H
#include <string>
#include <vector>
#include "Entity.h"

class Context {
public:
    std::string level;
    
    std::vector<std::vector<Entity*>> levelmatrix_ = std::vector<std::vector<Entity*>> (8,std::vector<Entity*>(8));

    /////These flags keep track of some states the game can be in, they are mosly used to skip large parts of the code when false
    /////this is done for optimalisation

    //Gets set to true when the currentmissile hits and get removed, leads to "loading" a new missile in the catapult and updating the missilequeue
    bool LoadNextMissile = false;
    //Gets set to true when a levelelement gets hit, leads to code that updates there status and removes them when nececery
    bool TargetsHit = false;
    //Gets set to true when levelements get removed, leads to a levelUpdate in the LevelSystem
    bool NeedLevelUpdate = false;
    
    int targetcounter;   // dit is om te weten wanneer het spel is afgelopen, komt hij op 0 dan is het level gedaan

    //basic timer, inceases by 1 every update
    int timer = 0;

    /////Replay information
    std::string replayFile;
    std::vector<std::string> missiles;
    std::vector<std::string> actions;
    int seed;

    bool replay = false;
    int inputPointer = 0;
};

#endif