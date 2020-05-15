#ifndef LAUNCHERSYSTEM_H
#define LAUNCHERSYSTEM_H

#include <vector>
#include <algorithm>

#include "Allkit.h"
#include "Point.h"
#include "System.h"

class LauncherSystem : public System {
    public:
        LauncherSystem(){
            ak_ = &Allkit::Get();
        }

        virtual ~LauncherSystem(){};
        
        void Update();

    private:
        Allkit* ak_ = NULL;

        bool MissileSelected(Entity*,Point,Engine::KEY_PRESSED);

        Point ConvertMouse(Point);

        Point ConvertPoint(Point);

        void LaunchMissile(Entity*,MissileQueueComponent*,Point);

        void UpdateQueue(std::set<Entity*>);

};

#endif