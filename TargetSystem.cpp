#include "TargetSystem.h"

void TargetSystem::Update() {

    EntityStream es = GetEngine()->GetEntityStream();
    Entity* currentMissile = *(es.WithTag(Component::CURRENTMISSILE).begin());
    std::set<Entity*> levelElements = es.WithTag(Component::LEVELELEMENT);

    if(GetEngine()->GetContext().TargetsHit){
        //Verwijderd Geraakte Targets indien nodig
        //Indien Geen geraakte targets meer: TargetsHit = false
        EvaluateTargets(levelElements);
    }

    if(currentMissile!=NULL){
        /*Collisionberekeningen worden enkel gedaan indien:
            -Er een missile bestaat
            -Deze zich rechts van de x 550 bevind en dus kan botsen met het level
        */
        PositionComponent* mpc= dynamic_cast<PositionComponent*>(currentMissile->GetComponent(Component::POSITION));
        if(mpc->position.x_>550){

            std::vector<Point> missilePoly;
            std::vector<Point> boxPoly;
            if(currentMissile->GetComponent(Component::MISSILE1)!=NULL){
                missilePoly = {mpc->position,Point(mpc->position.x_,mpc->position.y_+MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_)};
            }
            else if(currentMissile->GetComponent(Component::MISSILE2)!=NULL){
                missilePoly = {mpc->position,Point(mpc->position.x_,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_)};
            }
            else if(currentMissile->GetComponent(Component::MISSILE3)!=NULL){
                missilePoly = {mpc->position,Point(mpc->position.x_,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_-MISSILE_DST_HEIGHT),Point(mpc->position.x_+MISSILE_DST_WIDTH,mpc->position.y_)};
            }

            for(Entity* levelElement: levelElements){
                PositionComponent* bpc = dynamic_cast<PositionComponent*>(levelElement->GetComponent(Component::POSITION));
                boxPoly = {bpc->position,Point(bpc->position.x_,bpc->position.y_-MISSILE_DST_HEIGHT),Point(bpc->position.x_+MISSILE_DST_WIDTH,bpc->position.y_),Point(bpc->position.x_+MISSILE_DST_WIDTH,bpc->position.y_-MISSILE_DST_HEIGHT)};

                LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(levelElement->GetComponent(Component::LEVELELEMENT));

                if(CheckCollision(missilePoly,boxPoly)){
                    //Colision with box happend
                    lec->IsHit = true;
                    GetEngine()->GetContext().TargetsHit = true;
                    GetEngine()->GetContext().LoadNextMissile = true;
                }
            }
            if(GetEngine()->GetContext().TargetsHit){
                GetEngine()->RemoveEntity(currentMissile);
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
    std::cout<<"Evaluating target" << std::endl;
    int c = 0;
    for(Entity* levelElement:levelElements){
        LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(levelElement->GetComponent(Component::LEVELELEMENT));
        if(lec->IsHit){
            lec->HitCounter += 1;
            if(lec->HitCounter >= HITDURATION && (levelElement->HasComponent(Component::BOX) || levelElement->HasComponent(Component::TARGET))){
                GetEngine()->RemoveEntity(levelElement);
                GetEngine()->GetContext().levelmatrix_[lec->matrixPosition.x_][lec->matrixPosition.y_] = NULL;
                GetEngine()->GetContext().NeedLevelUpdate = true;
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
        //Geen targets meer die geraakt zijn
        GetEngine()->GetContext().TargetsHit = false;
    }
}