#ifndef MENU_H
#define MENU_H

#include "Allkit.h"
#include "Constants.h"
#include "Context.h"

class Button;

class Menu
{
public:
    Menu();

    // Start the menu loop
    void Run();

private:
    Allkit* ak_ = NULL;
    Sprite background = SPRT_MENU_BACKGROUND;
    std::vector<Button> MainMenuButtons;
    std::vector<Button> LevelSelectButtons;
    std::vector<Button> ReplaySelectButtons;
    unsigned int selectedButton = 0;
    Color unselected = Color(10,10,10);
    Color selected = Color(255,0,0);

    void StartGame(Context&);

    void UpdateMenu(std::vector<Button>);

    void LevelSelect();

    void ReplaySelect();

protected:
    bool exit_ = false;
};

class Button
{
public:
    std::string text;
    Point point;

    Button(std::string,Point);
};

#endif