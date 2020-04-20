#include "Game.h"

bool Game::Run() {
    exit_ = false;
    SetupSystems();
    LoadLevel();
    InitMissileQueue();


    ak_->StartTimer();
    while (!exit_)
    {
        ak_->NextEvent();


        if(ak_->IsSpaceBarPushed()){
            engine_.keyInput = Engine::KEY_SPACE;
        }
        else if(ak_->IsMouseClicked()){
            engine_.keyInput = Engine::KEY_MOUSE_DOWN;
        }
        else if(ak_->IsMouseReleased()){
            engine_.keyInput = Engine::KEY_MOUSE_UP;
        }
        else if(ak_->HasMouseMoved()){
            engine_.mouseinput = ak_->GetMouse();
        }


        if(ak_->IsTimerEvent()){
            engine_.Update();
            engine_.keyInput = Engine::KEY_NONE;
        }

        if(ak_->IsWindowClosed()){
            exit_ = true;
        }    
            
        if(ak_->IsArrowKeyDownPushed()){
            exit_= true;
        }
    }
    


    return true;
}

void Game::SetupSystems(){
    
    LauncherSystem* ls = new LauncherSystem();
    ls->SetEngine(&engine_);
    engine_.AddSystem(ls);

    MissileSystem* ms = new MissileSystem();
    ms->SetEngine(&engine_);
    engine_.AddSystem(ms);

    TargetSystem* ts = new TargetSystem();
    ts->SetEngine(&engine_);
    engine_.AddSystem(ts);

    RenderSystem* rs = new RenderSystem();
    rs->SetEngine(&engine_);
    engine_.AddSystem(rs);
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
    int x,y,rx,ry;
    
    while(inFile >> c){
        x = i%8;
        y = floor(i/8);
        rx = 600 + 35*x;
        ry = 125 + 35*y;
        if(c == 'B'){
            Entity* box = new Entity;
            box->Add(new BoxComponent(Point(rx,ry)));
            engine_.AddEntity(box);
            levelmatrix_[x][y] = box;
        }
        i++;
    }
    inFile.close();
}

void Game::InitMissileQueue(){
    Entity* me = new Entity;
    me->Add(new MissileComponent(Point(120,230)));
    me->Add(new Missile1Component());
    me->Add(new MissileQueueComponent(0));
    engine_.AddEntity(me);

    for(int i = 1;i<4;i++){
        Entity* me = new Entity;
        me->Add(new MissileComponent(Point(80 - 40*(i-1),125)));
        me->Add(new Missile1Component());
        me->Add(new MissileQueueComponent(i));
        engine_.AddEntity(me);

    }
}