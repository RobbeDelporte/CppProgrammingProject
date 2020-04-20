#ifndef TARGETSYSTEM_H
#define TARGETSYSTEM_H

#include <vector>
#include <iostream>

#include "Point.h"
#include "System.h"

class TargetSystem : public System {
    public:
        void Update();

    private:
        bool CheckCollision(std::vector<Point>& coordinates_poly_one, std::vector<Point>& coordinates_poly_two);

        std::vector<Point> GetEdges(std::vector<Point>& coordinates);

        void ProjectOnAxis(std::vector<Point>& coordinates, Point& axis, double& min, double& max);

        double DistanceBetweenPolys(double min_a, double max_a, double min_b, double max_b);

        void EvaluateTargets(std::set<Entity*>);
};

#endif