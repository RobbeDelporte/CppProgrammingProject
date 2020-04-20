#include "Context.h"
#include "Game.h"
#include "Menu.h"
#include <iostream>

Menu::Menu(){
    ak_ = &Allkit::Get();
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
    
    // Menu loop
    while (!exit_) {
        ak_->NextEvent();
        if(ak_->IsArrowKeyUpPushed()){
            selectedButton == 0? selectedButton=MainMenuButtons.size()-1: selectedButton-=1;
        }
        else if(ak_->IsArrowKeyDownPushed()){
            selectedButton == MainMenuButtons.size() - 1 ? selectedButton=0: selectedButton+=1;
        }
        else if(ak_->IsEnterKeyPushed()){
            if(selectedButton==0){
                LevelSelect();
            }
            else if(selectedButton==2){
                exit_ = true;
            }
        }

        UpdateMenu(MainMenuButtons);
        
        if (ak_->IsWindowClosed()) {
            exit_ = true;
        }
    }
}

void Menu::StartGame()
{
    std::cout << "StartButton Selected" << std::endl;
    Context context;
    context.level = "./assets/levels/levelTest.txt";

    Game game(context);
    
    game.Run();
}

void Menu::LevelSelect(){
    exit_ = false;
    selectedButton = 0;
    
    // Menu loop
    while (!exit_) {
        ak_->NextEvent();
        if(ak_->IsArrowKeyUpPushed()){
            selectedButton == 0? selectedButton=LevelSelectButtons.size()-1: selectedButton-=1;
        }
        else if(ak_->IsArrowKeyDownPushed()){
            selectedButton == LevelSelectButtons.size() - 1 ? selectedButton=0: selectedButton+=1;
        }
        else if(ak_->IsEnterKeyPushed()){
            if(selectedButton==0){
                StartGame();
            }
            else if(selectedButton==3){
                exit_ = true;
            }
        }

        UpdateMenu(LevelSelectButtons);
        
        if (ak_->IsWindowClosed()) {
            exit_ = true;
        }
    }

    exit_ = false;
    selectedButton = 0;
}

void Menu::UpdateMenu(std::vector<Button> buttons){
    ak_->DrawScaledBitmap(background,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    for(unsigned int i =0;i<buttons.size();i++){
        Color color;
        i == selectedButton ? color = selected : color = unselected;
        ak_->DrawString(buttons[i].text, buttons[i].point,color,Allkit::ALIGN_CENTER,false);
    }
    ak_->DrawOnScreen();
}

Button::Button(std::string t,Point p){
    text = t;
    point = p;
}
