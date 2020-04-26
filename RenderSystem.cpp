#include "RenderSystem.h"

void RenderSystem::Update(){
    ak_->DrawScaledBitmap(Sprite::SPRT_BACKGROUND,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT,0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

    EntityStream es = GetEngine()->GetEntityStream();
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
            GetEngine()->RemoveEntity(entity);
            delete entity;
        }
    }
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