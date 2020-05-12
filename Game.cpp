#include "Game.h"

bool Game::Run() {

    //some initializations
    exit_ = false;
    SetupSystems();
    LoadLevel();
    InitMissileQueue();
    ak_->LoadLaunchSound();
    ak_->StartTimer();

    if(engine_.GetContext().replay){
        ReadHighscoreFile();
    }

    //////MAIN GAME LOOP
    while (!exit_)
    {
        ak_->NextEvent();

        //The eventqueue gets read out as fast as possible
        //When the update us called, the most recent mouse and keyinput get passed on via the context
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

        //the update is called when the event is a timerevent
        if(ak_->IsTimerEvent()){
            engine_.Update();
            engine_.keyInput = Engine::KEY_NONE;
        }

        if(ak_->IsWindowClosed()){
            ak_->StopTimer();
            exit_ = true;
        }    
        //Was used for debugging so the game could quickly be restarted
        if(ak_->IsArrowKeyDownPushed()){
            ak_->StopTimer();
            exit_= true;
        }

        if (engine_.GetContext().targetcounter==0){
            engine_.Update();
            ak_->StopTimer();
            EndGame();
        }
        
    }
    
    return true;
}

void Game::EndGame(){
    if(not(engine_.GetContext().replay)){
        std::ofstream outfile;
        outfile.open("./assets/highscores/highscore1.txt");
        for(std::vector<std::string>::iterator it = engine_.GetContext().missiles.begin();it!=engine_.GetContext().missiles.end();++it){
            outfile << (*it) << std::endl;
        }
        outfile.close();
    }

    Point p(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-30);
    Color c(10,10,10);
    std::string s("<PRESS ENTER>");
    ak_->DrawString(s,p,c,Allkit::ALIGN_CENTER,true);
    ak_->DrawOnScreen();
    exit_ = false;
    while(!exit_){
        ak_->NextEvent();
        if(ak_->IsEnterKeyPushed() || ak_->IsWindowClosed()){
            exit_ = true;
        }
    }
}

void Game::ReadHighscoreFile(){
    std::ifstream infile;
    infile.open("./assets/highscores/highscore1.txt");
    std::string c;
    while(infile >> c){
        engine_.GetContext().missiles.push_back(c);
    }
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

    LevelSystem* lvls = new LevelSystem();
    lvls->SetEngine(&engine_);
    engine_.AddSystem(lvls);

    RenderSystem* rs = new RenderSystem();
    rs->SetEngine(&engine_);
    engine_.AddSystem(rs);
}

void Game::LoadLevel(){
    //Catapult things
    Entity* entity = new Entity;
    entity->Add(new SpriteComponent(SPRT_LAUNCHER,Point(100,235)));
    engine_.AddEntity(entity);

    std::ifstream inFile;
    inFile.open(context_.level);
    if(!inFile){
        std::cerr << "Unable to open file: "<<context_.level<<std::endl;
        exit(1); 
    }
    char c;
    int i =0;
    int mx,my,rx,ry;
    engine_.GetContext().targetcounter=0;
    while(inFile >> c){
        mx = i%8;
        my = 7 - floor(i/8);
        rx = 600 + 35*mx;
        ry = 125 + 35*my;

        Entity* entity = new Entity;
        entity->Add(new LevelElementComponent(Point(mx,my)));
        entity->Add(new PositionComponent(Point(rx,ry)));
        if(c =='B'){
            entity->Add(new BoxComponent());
            engine_.AddEntity(entity);
        }
        else if(c == 'S'){
            entity->Add(new StoneComponent());
            engine_.AddEntity(entity);
        }
        else if(c == 'T'){
            entity->Add(new TargetComponent());
            engine_.AddEntity(entity);
            engine_.GetContext().targetcounter+=1; //nieuw, telt alle targets, per verwijderde (dus voldoende geraakt) target gaat er weer 1 af            
        }                               // totdat targetconuter==0 , dan weer quit
        else{
            entity = NULL;
        }
         
        engine_.GetContext().levelmatrix_[mx][my] = entity;
        i++;
    }
    std::cout <<"aantaltargets"<<" "<<engine_.GetContext().targetcounter<< std::endl; //mag weg: gewoon voor debuggen
    inFile.close();
}

void Game::InitMissileQueue(){
    Entity* me = new Entity;
    me->Add(new PositionComponent(Point(140,220)));
    me->Add(new Missile1Component());
    me->Add(new MissileQueueComponent(0));
    engine_.AddEntity(me);

    int r;
    for(int i = 1;i<4;i++){
        Entity* me = new Entity;
        me->Add(new PositionComponent(Point(80 - 40*(i-1),125)));
        r = rand()%3;
        if(r == 0){
            me->Add(new Missile1Component());
        }
        else if(r == 1){
            me->Add(new Missile2Component());
        }
        else if(r == 2){
            me->Add(new Missile3Component());
        }
        me->Add(new MissileQueueComponent(i));
        engine_.AddEntity(me);
    }
}