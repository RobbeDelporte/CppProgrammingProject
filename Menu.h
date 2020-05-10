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

    void StartGame(Context&);

    void UpdateMenu(std::vector<Button>);

    void LevelSelect();

private:
    Allkit* ak_ = NULL;
    Sprite background = SPRT_MENU_BACKGROUND;
    std::vector<Button> MainMenuButtons;
    std::vector<Button> LevelSelectButtons;
    unsigned int selectedButton = 0;
    Color unselected = Color(10,10,10);
    Color selected = Color(255,0,0);
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