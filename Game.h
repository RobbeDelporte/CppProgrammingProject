#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "Allkit.h"
#include "Context.h"

class Game
{
public:
    Game(Context& context) : context_(context),
                             engine_(context) {
        ak_ = &Allkit::Get();
    };

    ~Game()
    {
        // TODO
    };

    bool Run();

private:
    Allkit* ak_ = NULL;
    Context& context_;
    Engine engine_;
};

#endif