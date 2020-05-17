#include "TargetSystem.h"

void TargetSystem::Update() {

    EntityStream es = engine_->GetEntityStream();
    Entity* currentMissile = *(es.WithTag(Component::CURRENTMISSILE).begin());
    std::set<Entity*> levelElements = es.WithTag(Component::LEVELELEMENT);

    if(engine_->GetContext().TargetsHit){
        //Deletes Hit target when necessary
        //When there are no hit targets left: TargetsHit = false
        EvaluateTargets(levelElements);
    }

    if(currentMissile!=NULL){
        /*Collisioncalculations only happen when:
            -A missile (in flight) exist
            -this missile exist in the right side of the game
        */
        PositionComponent* mpc= dynamic_cast<PositionComponent*>(currentMissile->GetComponent(Component::POSITION));
        CurrentMissileComponent* cmc= dynamic_cast<CurrentMissileComponent*>(currentMissile->GetComponent(Component::CURRENTMISSILE));

        if(mpc->position.x_>550){

            std::vector<Point> missilePoly;
            std::vector<Point> boxPoly;

            //missile Hitbox varies
            if(currentMissile->GetComponent(Component::MISSILE1)!=NULL){
                missilePoly = {mpc->position,Point(mpc->position.x_,mpc->position.y_+MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_)};
            }
            else if(currentMissile->GetComponent(Component::MISSILE2)!=NULL){
                missilePoly =  {Point(mpc->position.x_,mpc->position.y_-8),Point(mpc->position.x_,mpc->position.y_+4-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_+4-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-8)};
            }
            else if(currentMissile->GetComponent(Component::MISSILE3)!=NULL){
                missilePoly = {Point(mpc->position.x_+MISSILE_DST_WIDTH/2,mpc->position.y_),Point(mpc->position.x_,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-MISSILE_DST_HEIGHT)};
            }

            for(Entity* levelElement: levelElements){
                PositionComponent* bpc = dynamic_cast<PositionComponent*>(levelElement->GetComponent(Component::POSITION));
                boxPoly = {Point(bpc->position.x_+1,bpc->position.y_-1),Point(bpc->position.x_+1,bpc->position.y_-MISSILE_DST_HEIGHT+1),Point(bpc->position.x_+MISSILE_DST_WIDTH-1,bpc->position.y_-MISSILE_DST_HEIGHT+1),Point(bpc->position.x_+MISSILE_DST_WIDTH-1,bpc->position.y_-1)};

                LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(levelElement->GetComponent(Component::LEVELELEMENT));

                if(CheckCollision(missilePoly,boxPoly)){
                    //Colision with box happend
                    lec->IsHit = true;
                    engine_->GetContext().TargetsHit = true;
                    engine_->GetContext().LoadNextMissile = true;
                    
                    ////Logic for "hitting" more targets behind the collisiontarget
                    ////this is easily done by using the levelmatrix in the context, the position of the collisiontarget in this matrix (see levelelement component)
                    ////and the IsHit flag stored in the same component

                    //conditions are in the horizontal case:
                    //The collision is horizontal,  The horizonal speed is high enough,  The extra target is in bounds, The extra taget exists

                    //horizontal
                    std::cout << mpc->position.y_ << " " << bpc->position.y_ << " " << cmc->xVelocity << "   " << MISSILE_DST_HEIGHT/2 << std::endl;
                    if(abs(mpc->position.y_-bpc->position.y_)<MISSILE_DST_HEIGHT*(3/2) && cmc->xVelocity > 1000 && lec->matrixPosition.x_ <= 6 && engine_->GetContext().levelmatrix_[lec->matrixPosition.x_+1][lec->matrixPosition.y_] != NULL){
                        std::cout << "fast horizontal collision happend and is possible" << std::endl;
                        dynamic_cast<LevelElementComponent*>(engine_->GetContext().levelmatrix_[lec->matrixPosition.x_+1][lec->matrixPosition.y_]->GetComponent(Component::LEVELELEMENT))->IsHit = true;
                        if(cmc->xVelocity >= 1400 && lec->matrixPosition.x_<=5 && engine_->GetContext().levelmatrix_[lec->matrixPosition.x_+ 2][lec->matrixPosition.y_]){
                            dynamic_cast<LevelElementComponent*>(engine_->GetContext().levelmatrix_[lec->matrixPosition.x_+2][lec->matrixPosition.y_]->GetComponent(Component::LEVELELEMENT))->IsHit = true;
                        }
                    }
                    //vertical
                    if(abs(mpc->position.x_-bpc->position.x_)<MISSILE_DST_WIDTH*(3/2)&& cmc->yVelocity < -1000 && lec->matrixPosition.y_ >= 1 && engine_->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_-1] != NULL){
                        std::cout << "fast vertical collision happend and is possible" << std::endl;
                        dynamic_cast<LevelElementComponent*>(engine_->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_-1]->GetComponent(Component::LEVELELEMENT))->IsHit = true;
                        if(lec->matrixPosition.y_ >= 2 && engine_->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_-2] != NULL){
                            dynamic_cast<LevelElementComponent*>(engine_->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_-2]->GetComponent(Component::LEVELELEMENT))->IsHit = true;                    
                        }
                    }
                }
            }
            //explosion effect for missile1
            if(engine_->GetContext().TargetsHit){
                engine_->RemoveEntity(currentMissile);
                if(currentMissile->HasComponent(Component::MISSILE1)){
                    Entity* e = new Entity;
                    e->Add(new ExplosionEffectComponent(Point(mpc->position.x_,mpc->position.y_)));
                    engine_->AddEntity(e);
                }
                delete currentMissile;
            }
        }
    }
}

bool TargetSystem::CheckCollision(std::vector<Point>& coordinates_poly_one, std::vector<Point>& coordinates_poly_two) {
    bool collision = true;

    std::vector<Point> edges_poly_one = GetEdges(coordinates_poly_one);
    std::vector<Point> edges_poly_two = GetEdges(coordinates_poly_two);

    Point edge;

    // Loop through all the edges of both polygons
    for (std::size_t edge_id = 0; edge_id < edges_poly_one.size() + edges_poly_two.size(); edge_id++) {
        if (edge_id < edges_poly_one.size()) {
            edge = edges_poly_one[edge_id];
        }
        else {
            edge = edges_poly_two[edge_id - edges_poly_one.size()];
        }

        // Find perpendicular axis to current edge
        Point axis(-edge.y_, edge.x_);
        axis.Normalize();

        // Find projection of polygon on current axis
        double min_dotp_poly_one = 0;
        double max_dotp_poly_one = 0;
        double min_dotp_poly_two = 0;
        double max_dotp_poly_two = 0;
        
        ProjectOnAxis(coordinates_poly_one, axis, min_dotp_poly_one, max_dotp_poly_one);
        ProjectOnAxis(coordinates_poly_two, axis, min_dotp_poly_two, max_dotp_poly_two);

        // Check if polygon projections overlap
        if (DistanceBetweenPolys(min_dotp_poly_one, max_dotp_poly_one, min_dotp_poly_two, max_dotp_poly_two) > 0) {
            collision = false;
            break;
        }
    }

    return collision;
}

std::vector<Point> TargetSystem::GetEdges(std::vector<Point>& coordinates) {
    std::vector<Point> edges;
    Point prevPoint = coordinates[0];
    for (std::size_t i = 1; i < coordinates.size(); i++) {
        edges.push_back(coordinates[i] - prevPoint);
        prevPoint = coordinates[i];
    }
    edges.push_back(coordinates[0] - prevPoint);

    return edges;
}

void TargetSystem::ProjectOnAxis(std::vector<Point>& coordinates, Point& axis, double& min, double& max) {
    double dotp = coordinates[0] >> axis;
    min = dotp;
    max = dotp;
    for (std::size_t i = 0; i < coordinates.size(); i++) {
        dotp = coordinates[i] >> axis;
        if (dotp < min) {
            min = dotp;
        }
        else if (dotp > max) {
            max = dotp;
        }
    }
}

double TargetSystem::DistanceBetweenPolys(double min_dotp_poly_one, double max_dotp_poly_one, double min_dotp_poly_two, double max_dotp_poly_two) {
    if (min_dotp_poly_one < min_dotp_poly_two) {
        return min_dotp_poly_two - max_dotp_poly_one;
    }
    else {
        return min_dotp_poly_one - max_dotp_poly_two;
    }
}

void TargetSystem::EvaluateTargets(std::set<Entity*> levelElements){
    int c = 0;
    for(Entity* levelElement:levelElements){
        LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(levelElement->GetComponent(Component::LEVELELEMENT));
        if(lec->IsHit){
            lec->HitCounter += 1;
            if(lec->HitCounter >= HITDURATION && (levelElement->HasComponent(Component::BOX) || levelElement->HasComponent(Component::TARGET))){
                if (levelElement->HasComponent(Component::TARGET)){
                    engine_->GetContext().targetcounter-=1;//decreases targetcounter by one per target who has been hit enough
                    std::cout<<"aantal targets over:"<<" "<<engine_->GetContext().targetcounter<<std::endl;//prints the number of remaining targets in the terminal    
                }
                engine_->RemoveEntity(levelElement);
                engine_->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_] = NULL;
                engine_->GetContext().NeedLevelUpdate = true;
                delete levelElement;

            }
            else if(lec->HitCounter >= HITDURATION && levelElement->HasComponent(Component::STONE)){
                lec->IsHit = false;
                lec->HitCounter = 0;
            }
            else{
                c += 1;
            }
        }
    }
    if(c == 0){
        //No more targets who are hit
        engine_->GetContext().TargetsHit = false;
    }
}