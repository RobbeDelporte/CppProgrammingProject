#include "TargetSystem.h"

void TargetSystem::Update() {
    // TODO
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
