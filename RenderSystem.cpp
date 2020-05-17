#include "RenderSystem.h"

void RenderSystem::Update(){
    ak_->ClearScreen();
    ak_->DrawScaledBitmap(Sprite::SPRT_BACKGROUND,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

    EntityStream es = engine_->GetEntityStream();
    std::set<Entity*> entities;


    entities = es.WithTag(Component::SPRITE);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::SPRITE);
        SpriteComponent* sc = dynamic_cast<SpriteComponent*>(component);
        RenderSprite(sc);
    }

    entities = es.WithTag(Component::BOX);
    for(Entity* entity: entities){
        LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(entity->GetComponent(Component::LEVELELEMENT));
        PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        RenderBox(pc,lec);
    }

    entities = es.WithTag(Component::STONE);
    for(Entity* entity: entities){
        LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(entity->GetComponent(Component::LEVELELEMENT));
        PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        RenderStone(pc,lec);
    }

    entities = es.WithTag(Component::TARGET);
    for(Entity* entity: entities){
        LevelElementComponent* lec = dynamic_cast<LevelElementComponent*>(entity->GetComponent(Component::LEVELELEMENT));
        PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
        TargetComponent* tc = dynamic_cast<TargetComponent*>(entity->GetComponent(Component::TARGET));

        RenderTarget(pc,lec,tc);
    }

    entities = es.WithTag(Component::MISSILE1);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::POSITION);
        PositionComponent* pc = dynamic_cast<PositionComponent*>(component);
        RenderMissile1(pc);
    }

    entities = es.WithTag(Component::MISSILE2);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::POSITION);
        PositionComponent* pc = dynamic_cast<PositionComponent*>(component);
        Missile2Component* mc = dynamic_cast<Missile2Component*>(entity->GetComponent(Component::MISSILE2));
        RenderMissile2(pc,mc);
    }

    entities = es.WithTag(Component::MISSILE3);
    for(Entity* entity: entities){
        Component* component = entity->GetComponent(Component::POSITION);
        PositionComponent* pc = dynamic_cast<PositionComponent*>(component);
        RenderMissile3(pc);
    }

    entities = es.WithTag(Component::EXPLOSIONEFFECT);
    for(Entity* entity: entities){
        ExplosionEffectComponent* eec = dynamic_cast<ExplosionEffectComponent*>(entity->GetComponent(Component::EXPLOSIONEFFECT));
        if(RenderExplosion(eec)){
            engine_->RemoveEntity(entity);
            delete entity;
        }
    }

    if(DEBUG_MODE==true){
        std::set<Component::Tag> tags = {Component::BOX,Component::STONE,Component::TARGET};
        entities = es.WithTagsOR(tags);
        for(Entity* entity:entities){
            PositionComponent* bpc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
                std::vector<Point> boxPoly = {Point(bpc->position.x_+1,SCREEN_HEIGHT-(bpc->position.y_-1)),Point(bpc->position.x_+1,SCREEN_HEIGHT-(bpc->position.y_-MISSILE_DST_HEIGHT+1)),Point(bpc->position.x_+MISSILE_DST_WIDTH-1,SCREEN_HEIGHT-(bpc->position.y_-MISSILE_DST_HEIGHT+1)),Point(bpc->position.x_+MISSILE_DST_WIDTH-1,SCREEN_HEIGHT-(bpc->position.y_-1))};
            ak_->DrawPoly(boxPoly);
        }

        entities = es.WithTag(Component::MISSILE1);
        for(Entity* entity:entities){
            PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
            std::vector<Point> poly = {Point(pc->position.x_,SCREEN_HEIGHT-pc->position.y_),Point(pc->position.x_,SCREEN_HEIGHT-pc->position.y_+MISSILE_DST_HEIGHT),Point(pc->position.x_+MISSILE_DST_WIDTH,SCREEN_HEIGHT-pc->position.y_+MISSILE_DST_HEIGHT),Point(pc->position.x_+MISSILE_DST_WIDTH,SCREEN_HEIGHT-pc->position.y_)};
            ak_->DrawPoly(poly);
        }

        entities = es.WithTag(Component::MISSILE2);
        for(Entity* entity:entities){
            PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
            std::vector<Point> poly = {Point(pc->position.x_,SCREEN_HEIGHT-pc->position.y_+8),Point(pc->position.x_,SCREEN_HEIGHT-pc->position.y_-4+MISSILE_DST_HEIGHT),Point(pc->position.x_+MISSILE_DST_WIDTH,SCREEN_HEIGHT-pc->position.y_-4+MISSILE_DST_HEIGHT),Point(pc->position.x_+MISSILE_DST_WIDTH,SCREEN_HEIGHT-pc->position.y_+8)};
            ak_->DrawPoly(poly);
        }

        entities = es.WithTag(Component::MISSILE3);
        for(Entity* entity:entities){
            PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
            std::vector<Point> poly = {Point(pc->position.x_+MISSILE_DST_WIDTH/2,SCREEN_HEIGHT-pc->position.y_),Point(pc->position.x_,SCREEN_HEIGHT-pc->position.y_+MISSILE_DST_HEIGHT),Point(pc->position.x_+MISSILE_DST_WIDTH,SCREEN_HEIGHT-pc->position.y_+MISSILE_DST_HEIGHT)};
            ak_->DrawPoly(poly);
        }
    }

    entities = es.WithTag(Component::MISSILEQUEUE);
    bool controlflag = false;
    for (Entity* entity:entities){
        MissileQueueComponent* mqc = dynamic_cast<MissileQueueComponent*>(entity->GetComponent(Component::MISSILEQUEUE));
        if (mqc->queuenumber==0){
            PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
            Render_elastic_attached(pc);
            controlflag = true;
            break;
        }
    }
    if (controlflag==false){
        Render_elastic_not_attached();
    }

    Point p(10,10);
    Color c(10,10,10);
    ak_->DrawString(std::string("Time: ") += (std::to_string(engine_->GetContext().timer/60)),p,c,Allkit::ALIGN_LEFT,true);

    ak_->DrawOnScreen();
}


void RenderSystem::RenderSprite(SpriteComponent* sc){
    ak_->DrawScaledBitmap(sc->sprite,0,0,LAUNCHER_SRC_WIDTH,LAUNCHER_SRC_HEIGHT,sc->position.x_,SCREEN_HEIGHT-sc->position.y_,LAUNCHER_DST_WIDTH,LAUNCHER_DST_HEIGHT);
}

void RenderSystem::RenderBox(PositionComponent* pc,LevelElementComponent* lec){
    Sprite s;
    lec->IsHit ? s=SPRT_CSTONE_HIT : s = SPRT_CSTONE; 
    
    ak_->DrawScaledBitmap(s,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

void RenderSystem::RenderStone(PositionComponent* pc,LevelElementComponent* lec){
    Sprite s;
    lec->IsHit ? s=SPRT_OBSIDIAN_HIT : s = SPRT_OBSIDIAN; 
    
    ak_->DrawScaledBitmap(s,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

void RenderSystem::RenderTarget(PositionComponent* pc,LevelElementComponent* lec,TargetComponent* tc){
    Sprite s;
    lec->IsHit ? s=SPRT_SLIME_HIT : s = tc->TargetAnim[tc->animStage]; 
    ak_->DrawScaledBitmap(s,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);

    if(tc->animTiming>=TARGETANIMTIMING){
        tc->animStage = (tc->animStage+1)%(tc->TargetAnim.size());
        tc->animTiming = 0;
    }
    else{
        tc->animTiming += 1;
    }
}

void RenderSystem::RenderMissile1(PositionComponent* pc){
    ak_->DrawScaledBitmap(SPRT_FIRECHARGE,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

void RenderSystem::RenderMissile2(PositionComponent* pc,Missile2Component* mc){
    ak_->DrawScaledBitmap(mc->Missile2Anim[mc->animStage],0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
    if(mc->animTiming>=MISSILE2ANIMTIMING){
        mc->animStage = (mc->animStage+1)%(mc->Missile2Anim.size());
        mc->animTiming = 0;
    }
    else{
        mc->animTiming += 1;
    }
}

void RenderSystem::RenderMissile3(PositionComponent* pc){
    ak_->DrawScaledBitmap(SPRT_ROCK,0,0,MISSILE_SRC_WIDTH,MISSILE_SRC_HEIGHT,pc->position.x_,SCREEN_HEIGHT-pc->position.y_,MISSILE_DST_WIDTH,MISSILE_DST_HEIGHT);
}

bool RenderSystem::RenderExplosion(ExplosionEffectComponent* eec){
    ak_->DrawScaledBitmap(eec->ExplosionAnim[eec->animStage],0,0,50,50,eec->position.x_-17,SCREEN_HEIGHT-eec->position.y_-17,70,70);
    if(eec->animTiming>=EXPLOSIONANIMTIMING){
        eec->animStage = (eec->animStage+1);
        eec->animTiming = 0;
        if((eec->animStage == eec->ExplosionAnim.size())){
            return true;
        }
    }   
    else{
        eec->animTiming += 1;
    }
    return false;
}
void RenderSystem::Render_elastic_not_attached(void){
    Point a = Point(116, 220);  //a= punt vast aan linkertak vanboven 
    Point b = Point(125,257);  //b = knoop vast aan linkertak vanonder
    Point c = Point(199,250); //c= punt vast aan rechtertak vanboven
    Point d = Point(196,270); //d=punt vast aan rechtertak vanonder

    ak_->DrawLine(a,c); //we verbinden de bovenste twee punten
    ak_->DrawLine(b,d); //verbinde de onderste twee punten
}
void RenderSystem::Render_elastic_attached(PositionComponent* pc){
    Point a = Point(116, 220);  //a= punt vast aan linkertak vanboven 
    Point b = Point(125,257);  //b = knoop vast aan linkertak vanonder
    Point c = Point(199,250); //c= punt vast aan rechtertak vanboven
    Point d = Point(196,270); //d=punt vast aan rechtertak vanonder
    int xco = pc->position.x_+MISSILE_DST_WIDTH/2 ;
    int yco_boven = SCREEN_HEIGHT-pc->position.y_;
    int yco_beneden = SCREEN_HEIGHT-pc->position.y_+MISSILE_DST_HEIGHT;
    Point p1 = Point(xco, yco_boven);
    Point p2 = Point(xco, yco_beneden);
    ak_->DrawLine(a, p1);
    ak_->DrawLine(c, p1);
    ak_->DrawLine(b, p2);
    ak_->DrawLine(d, p2);
}