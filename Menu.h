#ifndef MENU_H
#define MENU_H

#include "Allkit.h"

class Menu
{
public:
    Menu() :
        ak_(& Allkit::Get())
    {};

    // Start the menu loop
    void Run();

    void StartGame();

private:
    Allkit* ak_ = NULL;

protected:
    bool exit_ = false;

};

#endif