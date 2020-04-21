#ifndef MISSILESYSTEM_H
#define MISSILESYSTEM_H

#include "Point.h"
#include "System.h"
#include <iostream>

class MissileSystem : public System {
    public:
        MissileSystem(){}

        virtual ~MissileSystem(){};

        void Update();

    private:
        Point UpdatePosition(CurrentMissileComponent*,Point,Entity*);

        void UpdateSpeed(CurrentMissileComponent*);

        void ActivateSpecial(Entity*);
};

#endif