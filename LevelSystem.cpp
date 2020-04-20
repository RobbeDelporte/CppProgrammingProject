#include "LevelSystem.h"

void LevelSystem::Update(){
    if(GetEngine()->GetContext().NeedLevelUpdate){
        std::vector<std::vector<Entity*>> levelmatrix = GetEngine()->GetContext().levelmatrix_;
        for(int x = 0;x<8;x++){
            for(int y = 7;y>=0;y--){
                if(levelmatrix[x][y]==NULL){
                    for(int yi = y + 1;yi<8;yi++){
                        if(levelmatrix[x][yi]!=NULL){
                            PositionComponent* pc = dynamic_cast<PositionComponent*>(levelmatrix[x][yi]->GetComponent(Component::POSITION));
                            LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(levelmatrix[x][yi]->GetComponent(Component::LEVELELEMENT));
                            lec->matrixPosition = lec->matrixPosition - Point(0,1);
                            pc->position = pc->position - Point(0,35);
                        }
                        levelmatrix[x][yi-1] = levelmatrix[x][yi];
                    }
                    levelmatrix[x][7] = NULL;
                }
            }
        }
        GetEngine()->GetContext().levelmatrix_ = levelmatrix;
        GetEngine()->GetContext().NeedLevelUpdate = false;
    }
}
