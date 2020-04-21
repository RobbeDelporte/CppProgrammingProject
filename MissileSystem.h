#ifndef MISSILESYSTEM_H
#define MISSILESYSTEM_H

#include "Point.h"
#include "System.h"
#include <iostream>

class MissileSystem : public System {
    public:
        MissileSystem(){
        }
        
        void Update();

    private:
        Point UpdatePosition(CurrentMissileComponent*,Point);

        void UpdateSpeed(CurrentMissileComponent*);

        void ActivateSpecial(Entity*);
};

#endif