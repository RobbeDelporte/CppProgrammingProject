#include "Game.h"

bool Game::Run() {
    
    //some initializations
    engine_.GetContext().replay = context_.replay;
    engine_.GetContext().level = context_.level;
    engine_.GetContext().replayFile = context_.replayFile;
    exit_ = false;
    SetupSystems();

    if(context_.replay){
        if(not(ReadHighscoreFile())){
            return true;
        }
    }

    LoadLevel();
    InitMissileQueue();
    ak_->LoadLaunchSound();
    ak_->StartTimer();


    //////MAIN GAME LOOP
    while (!exit_)
    {
        ak_->NextEvent();

        //The eventqueue gets read out as fast as possible
        //When the update is called, the most recent mouse and keyinput get passed on via the context
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
            engine_.GetContext().timer += 1;
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
    Color c(10,10,10);

    if(not(context_.replay)){
        //Get the highscore of the respective highscorefile
        std::ifstream inFile;
        if(engine_.GetContext().level == std::string("./assets/levels/level1.txt")){
            inFile.open("./assets/highscores/highscore_1.txt");
        }
        else if(engine_.GetContext().level == std::string("./assets/levels/level2.txt")){
            inFile.open("./assets/highscores/highscore_2.txt");
        }
        else if(engine_.GetContext().level == std::string("./assets/levels/level3.txt")){
            inFile.open("./assets/highscores/highscore_3.txt");
        }
        int highscore = 99999;
        for(std::string line; getline(inFile,line);){
            if(line == std::string("[SCORE]")){
                getline(inFile,line);
                std::stringstream ss;
                ss << line;
                int s;
                ss >> s;
                highscore = s;
            }
        }
        inFile.close();

        //If you got a beter score in the played game overwrite this highscore
        if(engine_.GetContext().timer / 60 < highscore){
            std::ofstream outFile;
            if(engine_.GetContext().level == std::string("./assets/levels/level1.txt")){
                outFile.open("./assets/highscores/highscore_1.txt");
            }
            else if(engine_.GetContext().level == std::string("./assets/levels/level2.txt")){
                outFile.open("./assets/highscores/highscore_2.txt");
            }
            else if(engine_.GetContext().level == std::string("./assets/levels/level3.txt")){
                outFile.open("./assets/highscores/highscore_3.txt");
            }
            outFile << "[LEVEL]" << std::endl;
            outFile << engine_.GetContext().level << std::endl;
            outFile << "[SEED]" << std::endl;
            outFile << std::to_string(engine_.GetContext().seed) << std::endl;
            outFile << "[MISSILES]" << std::endl;
            for(std::vector<std::string>::iterator it = engine_.GetContext().missiles.begin();it!=engine_.GetContext().missiles.end();++it){
                outFile << (*it) << std::endl;
            }
            outFile << "[ACTIONS]" << std::endl;
            for(std::vector<std::string>::iterator it = engine_.GetContext().actions.begin();it!=engine_.GetContext().actions.end();++it){
                outFile << (*it) << std::endl;
            }
            outFile << "[SCORE]"  << std::endl;
            outFile << std::to_string(engine_.GetContext().timer / 60) << std::endl;
            outFile.close();

            Point hp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+100);
            std::string h("NEW HIGHSCORE");
            ak_->DrawString(h,hp,c,Allkit::ALIGN_CENTER,true);
        }
    }

    //PRESS ENTER to continue code
    Point p(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-30);
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

bool Game::ReadHighscoreFile(){
    std::ifstream infile;
    infile.open(engine_.GetContext().replayFile);

    if(!infile){
        std::cerr << "Unable to open replayFile: "<<engine_.GetContext().replayFile<<std::endl;
        return false;
    }

    enum reading{
        none,
        missiles,
        actions
    }
    reading = none;
    for(std::string line; getline(infile,line);){
        if(line == std::string("[MISSILES]")){
            reading = missiles;
        }
        else if(line == std::string("[ACTIONS]")){
            reading = actions;
        }
        else if(line == std::string("[LEVEL]")){
            getline(infile,line);
            engine_.GetContext().level = line;
        }
        else if(line == std::string("[SCORE]")){
            getline(infile,line);
            //Do something with score
        }
        else if(line == std::string("[SEED]")){
            getline(infile,line);
            std::stringstream ss;
            ss << line;
            int s;
            ss >> s;
            engine_.GetContext().seed = s;
        }
        else{
            switch(reading){
            case missiles:
                engine_.GetContext().missiles.push_back(line);
                break;
            case actions:
                engine_.GetContext().actions.push_back(line);
            }
        }
    }
    infile.close();
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
    inFile.open(engine_.GetContext().level);
    if(!inFile){
        std::cerr << "Unable to open levelFile: "<<engine_.GetContext().level<<std::endl;
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
            engine_.GetContext().targetcounter+=1; //counts all targets in a level          
        }                               
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
    if(engine_.GetContext().replay){
        srand(engine_.GetContext().seed);
    }
    else{
        int r = rand()%1000;
        engine_.GetContext().seed = r;
        srand(r);
    }
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