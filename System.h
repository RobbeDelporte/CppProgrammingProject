#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"
#include "Engine.h"
#include "Constants.h"

class Engine;

class System {
public:
    virtual void Update() = 0;

    void SetEngine(Engine* engine){engine_ = engine;};

    Engine* GetEngine(){return engine_;}

private:
    Engine* engine_;
};

#endif