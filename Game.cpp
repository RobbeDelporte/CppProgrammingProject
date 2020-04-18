#include "Game.h"

bool Game::Run() {
    exit_ = false;
    SetupSystems();
    LoadLevel();

    Entity* teste = new Entity;
    teste->Add(new Missile1Component(Point(200,200)));
    engine_.AddEntity(teste);

    ak_->StartTimer();
    while (!exit_)
    {
        ak_->NextEvent();
        if(ak_->IsWindowClosed()){
            exit_ = true;

        }
        engine_.Update();

        if(ak_->IsWindowClosed()){
            exit_ = true;

        }
    }
    


    return true;
}

void Game::SetupSystems(){
    RenderSystem* rs = new RenderSystem();
    rs->SetEngine(&engine_);
    engine_.AddSystem(rs);
    
    LauncherSystem* ls = new LauncherSystem();
    ls->SetEngine(&engine_);
    engine_.AddSystem(ls);
}

void Game::LoadLevel(){
    std::ifstream inFile;
    inFile.open(context_.level);
    if(!inFile){
        std::cerr << "Unable to open file: "<<context_.level<<std::endl;
        exit(1); 
    }

    char c;
    int i =0;
    int x;
    int y;
    while(inFile >> c){
        x = i%8;
        y = floor(i/8);
        if(c == 'B'){
            Entity* box = new Entity;
            box->Add(new BoxComponent(x,y));
            engine_.AddEntity(box);
            levelmatrix_[x][y] = box;
        }
        i++;
    }
    inFile.close();
}