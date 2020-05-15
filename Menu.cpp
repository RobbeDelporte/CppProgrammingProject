#include "Game.h"
#include "Menu.h"
#include <iostream>

Menu::Menu(){
    ak_ = &Allkit::Get();

    //////GENERALIZATION OF MENU
    //A new menu consist of a vector with the menubuttons stored in the menu class
    //A function implements a way to select the buttons (this varies with te relative position of the buttons)
    // and some code that runs when a button is selected
    // UpdateMenu() takes as argument the button vector and draws the menu, only gets called after an update happens
    
    //Add buttons to respective ButtonVector
    MainMenuButtons.push_back(Button("START",Point(450,100)));
    MainMenuButtons.push_back(Button("REPLAY",Point(450,200)));
    MainMenuButtons.push_back(Button("QUIT",Point(450,400)));
    
    LevelSelectButtons.push_back(Button("LEVEL 1",Point(450,100)));
    LevelSelectButtons.push_back(Button("LEVEL 2",Point(450,200)));
    LevelSelectButtons.push_back(Button("LEVEL 3",Point(450,300)));
    LevelSelectButtons.push_back(Button("RETURN",Point(450,400)));
}

void Menu::Run()
{
    exit_ = false;
    UpdateMenu(MainMenuButtons);
    
    // Main Menu loop
    while (!exit_) {
        ak_->NextEvent();
        if(ak_->IsArrowKeyUpPushed()){
            selectedButton == 0? selectedButton=MainMenuButtons.size()-1: selectedButton-=1;
            UpdateMenu(MainMenuButtons);
        }
        else if(ak_->IsArrowKeyDownPushed()){
            selectedButton == MainMenuButtons.size() - 1 ? selectedButton=0: selectedButton+=1;
            UpdateMenu(MainMenuButtons);
        }
        else if(ak_->IsEnterKeyPushed()){
            if(selectedButton==0){
                LevelSelect();
            }
            else if(selectedButton==1){
                Context context;
                context.replay = true;
                StartGame(context);
            }
            else if(selectedButton==2){
                exit_ = true;
            }
            UpdateMenu(MainMenuButtons);
        }
        
        if (ak_->IsWindowClosed()) {
            exit_ = true;
        }
    }
}

void Menu::StartGame(Context& context) 
{
    std::cout << "StartButton Selected" << std::endl;


    Game game(context);
    
    game.Run();
    
}

void Menu::LevelSelect(){
    exit_ = false;
    selectedButton = 0;
    UpdateMenu(LevelSelectButtons);
    
    // Level Menu loop
    while (!exit_) {
        ak_->NextEvent();
        if(ak_->IsArrowKeyUpPushed()){
            selectedButton == 0? selectedButton=LevelSelectButtons.size()-1: selectedButton-=1;
            UpdateMenu(LevelSelectButtons);
        }
        else if(ak_->IsArrowKeyDownPushed()){
            selectedButton == LevelSelectButtons.size() - 1 ? selectedButton=0: selectedButton+=1;
            UpdateMenu(LevelSelectButtons);
        }
        else if(ak_->IsEnterKeyPushed()){
            if(selectedButton==0){
                Context context;
                context.level = "./assets/levels/level1.txt";
                StartGame(context);
            }
            else if(selectedButton==1){
                Context context;
                context.level = "./assets/levels/level2.txt";
                StartGame(context);
            }
            else if(selectedButton==2){
                Context context;
                context.level = "./assets/levels/levelTest.txt";
                StartGame(context);
            }
            else if(selectedButton==3){
                exit_ = true;
            }
            UpdateMenu(LevelSelectButtons);
        }
        
        if (ak_->IsWindowClosed()) {
            exit_ = true;
        }
    }

    exit_ = false;
    selectedButton = 0;
}

//Update Screen with correct buttons
void Menu::UpdateMenu(std::vector<Button> buttons){
    ak_->DrawScaledBitmap(background,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    for(unsigned int i =0;i<buttons.size();i++){
        Color color;
        i == selectedButton ? color = selected : color = unselected;
        ak_->DrawString(buttons[i].text, buttons[i].point,color,Allkit::ALIGN_CENTER,false);
    }
    ak_->DrawOnScreen();
}

//Simpel Button class
Button::Button(std::string t,Point p){
    text = t;
    point = p;
}
