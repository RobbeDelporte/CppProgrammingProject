#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "Allkit.h"
#include "Context.h"
#include "RenderSystem.h"
#include "LauncherSystem.h"
#include <fstream>
#include <iostream>


class Game
{
public:
    Game(Context& context) : context_(context), engine_(context) {
        ak_ = &Allkit::Get();
        levelmatrix_ = std::vector<std::vector<Entity*>> (8,std::vector<Entity*>(8));
    };

    ~Game()
    {
        // TODO
    };

    bool Run();

    void SetupSystems();

    void ReadLevelMatrix();

    void LoadLevel();

private:
    Allkit* ak_ = NULL;
    Context& context_;
    Engine engine_;
    bool exit_ = false;
public:
    std::vector<std::vector<Entity*>> levelmatrix_;
};

#endif