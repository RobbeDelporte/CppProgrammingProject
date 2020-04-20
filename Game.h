#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "Allkit.h"
#include "Context.h"
#include "RenderSystem.h"
#include "LauncherSystem.h"
#include "MissileSystem.h"
#include "TargetSystem.h"
#include "LevelSystem.h"

#include <fstream>
#include <iostream>


class Game
{
public:
    Game(Context& context) : context_(context), engine_(context) {
        ak_ = &Allkit::Get();
    };

    ~Game()
    {
        // TODO
    };

    bool Run();

    void SetupSystems();

    void ReadLevelMatrix();

    void LoadLevel();

    void InitMissileQueue();

private:
    Allkit* ak_ = NULL;
    Context& context_;
    Engine engine_;
    bool exit_ = false;
};

#endif