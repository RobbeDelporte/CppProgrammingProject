#include "TargetSystem.h"

void TargetSystem::Update() {

    EntityStream es = GetEngine()->GetEntityStream();
    Entity* currentMissile = *(es.WithTag(Component::CURRENTMISSILE).begin());
    std::set<Entity*> boxTargets = es.WithTag(Component::BOX);

    if(GetEngine()->GetContext().TargetsHit){
        //Verwijderd Geraakte Targets indien nodig
        //Indien Geen geraakte targets meer: TargetsHit = false
        EvaluateTargets(boxTargets);
    }

    if(currentMissile!=NULL){
        /*Collisionberekeningen worden enkel gedaan indien:
            -Er een missile bestaat
            -Deze zich rechts van de x 550 bevind en dus kan botsen met het level
        */
        MissileComponent* mc= dynamic_cast<MissileComponent*>(currentMissile->GetComponent(Component::MISSILE));
        if(mc->position.x_>550){

            std::vector<Point> missilePoly;
            std::vector<Point> boxPoly;
            if(currentMissile->GetComponent(Component::MISSILE1)!=NULL){
                missilePoly = {mc->position,Point(mc->position.x_,mc->position.y_-MISSILE_DST_HEIGHT),Point(mc->position.x_+MISSILE_DST_WIDTH,mc->position.y_),Point(mc->position.x_+MISSILE_DST_WIDTH,mc->position.y_-MISSILE_DST_HEIGHT)};
            }

            for(Entity* boxTarget: boxTargets){
                BoxComponent* bc = dynamic_cast<BoxComponent*>(boxTarget->GetComponent(Component::BOX));
                boxPoly = {bc->position,Point(bc->position.x_,bc->position.y_-MISSILE_DST_HEIGHT),Point(bc->position.x_+MISSILE_DST_WIDTH,bc->position.y_),Point(bc->position.x_+MISSILE_DST_WIDTH,bc->position.y_-MISSILE_DST_HEIGHT)};

                if(CheckCollision(missilePoly,boxPoly)){
                    //Colision with box happend
                    bc->BoxHit = true;
                    GetEngine()->GetContext().TargetsHit = true;
                    GetEngine()->RemoveEntity(currentMissile);
                    GetEngine()->GetContext().LoadNextMissile = true;
                }
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

void TargetSystem::EvaluateTargets(std::set<Entity*> boxTargets){
    std::cout<<"Evaluating target" << std::endl;
    int c = 0;
    for(Entity* boxTarget:boxTargets){
        BoxComponent* bc = dynamic_cast<BoxComponent*>(boxTarget->GetComponent(Component::BOX));
        if(bc->BoxHit){
            bc->HitCounter += 1;
            if(bc->HitCounter >= HITDURATION){
                GetEngine()->RemoveEntity(boxTarget);
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