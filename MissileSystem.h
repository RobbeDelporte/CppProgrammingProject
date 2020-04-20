#ifndef MISSILESYSTEM_H
#define MISSILESYSTEM_H

#include "Point.h"
#include "System.h"

class MissileSystem : public System {
    public:
        MissileSystem(){
        }
        
        void Update();

    private:
        Point UpdatePosition(CurrentMissileComponent*,Point);

        void UpdateSpeed(CurrentMissileComponent*);
};

#endif