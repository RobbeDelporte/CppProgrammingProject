#ifndef LAUNCHERSYSTEM_H
#define LAUNCHERSYSTEM_H

#include <vector>

#include "Allkit.h"
#include "Point.h"
#include "System.h"

class LauncherSystem : public System {
    public:
        LauncherSystem(){
            ak_ = &Allkit::Get();
        }
        void Update();

    private:
        Allkit* ak_ = NULL;

        bool Missile1Selected(Missile1Component*);

        Point ConvertMouse(Point);
};

#endif