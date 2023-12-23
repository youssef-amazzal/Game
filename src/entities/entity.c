#include "entity.h"
#include "assets.h"
#include <stdlib.h>
#include <stdio.h>

Entity *ENTITY_RECORD[MAX_ENTITIES];
int LAST_ID = 0;

static void initHitBox(Entity *e);

static void Update(Entity *e);
static void Animate(Entity *e);
static void Render(Entity *e);
static void React(Entity *e);
static void Free(Entity *e);

static bool IsMoving(Entity *e);
static void SetDestination(Entity *e, float x, float y);

static void PushReaction(Entity *pusher, Entity *pushed, void (*setDestination)(Entity *pushed, float x, float y));
static void BlockReaction(Entity *blocker, Entity *blocked, void (*setDestination)(Entity *blocked, float x, float y));

Entity *CreateEntity() 
{
    Entity *e = malloc(sizeof(Entity));
    e->id = LAST_ID++;
    e->child = NULL;
    
    initHitBox(e);
    
    e->spriteSheet = NULL;
    e->frameTexture = (Rectangle){0, 0, TILE_SIZE, TILE_SIZE};

    e->zIndex = 0;
    
    e->destFrame = (Rectangle){0, 0, TILE_SIZE * SCALING_FACTOR, TILE_SIZE * SCALING_FACTOR};
    e->origin = (Vector2){0, 0};
    e->velocity = (Vector2){0, 0};
    e->angle = 0;

    e->Update = Update;
    e->Animate = Animate;
    e->Render = Render;
    e->React = React;
    e->Free = Free;

    e->IsMoving = IsMoving;
    e->SetDestination = SetDestination;
    
    ENTITY_RECORD[e->id] = e;
    return e;
}

static void initHitBox(Entity *e)
{
    e->hitBox.owner = e->id;
    e->hitBox.isDisabled = false;

    e->hitBox.canBlock = false;
    e->hitBox.canBeBlocked = false;

    e->hitBox.canPush = false;
    e->hitBox.canBePushed = false;

    e->hitBox.canDestroy = false;
    e->hitBox.canBeDestroyed = false;

    e->hitBox.canCollect = false;
    e->hitBox.canBeCollected = false;

    e->hitBox.canTeleport = false;
    e->hitBox.canBeTeleported = false;

    e->hitBox.canBounce = false;
    e->hitBox.canBeBounced = false;
}

/************************
 * Core Funcs
 ************************/ 

static void Update(Entity *e) {}

static void Animate(Entity *e){}

static void Render(Entity *e)
{
    // Render the entity texture
    DrawTexturePro(*(e->spriteSheet), e->frameTexture, e->destFrame, e->origin, 0, WHITE);
    

    // Render the hitBox and the collisions
    static bool showcollision = false;
    
    if (showcollision) {
        DrawRectangleLinesEx(e->hitBox.area, 1, e->hitBox.color);

        for (int i = 0; i < e->id; i++) {
            
            Entity *this = e;
            Entity *other = ENTITY_RECORD[i];

            if (other != NULL) {
                Collision *collision = GetCollision(this->id, other->id);
                
                Color collisionColor = (Color){255, 180, 0, 120};
                Color blockingColor = (Color){255, 0, 0, 120};
                Color pushingColor = (Color){0, 255, 0, 120};
                Color destroyingColor = (Color){0, 0, 255, 120};
                Color collectingColor = (Color){255, 0, 255, 120};
                Color teleportingColor = (Color){0, 255, 255, 120};
                Color bouncingColor = (Color){255, 255, 0, 120};

                if (collision->isColliding) {

                    if (this->hitBox.canBlock && other->hitBox.canBeBlocked || other->hitBox.canBlock && this->hitBox.canBeBlocked) DrawRectangleRec(collision->area, blockingColor);
                    else if (this->hitBox.canPush && other->hitBox.canBePushed || other->hitBox.canPush && this->hitBox.canBePushed) DrawRectangleRec(collision->area, pushingColor);
                    else if (this->hitBox.canDestroy && other->hitBox.canBeDestroyed || other->hitBox.canDestroy && this->hitBox.canBeDestroyed) DrawRectangleRec(collision->area, destroyingColor);
                    else if (this->hitBox.canCollect && other->hitBox.canBeCollected || other->hitBox.canCollect && this->hitBox.canBeCollected) DrawRectangleRec(collision->area, collectingColor);
                    else if (this->hitBox.canTeleport && other->hitBox.canBeTeleported || other->hitBox.canTeleport && this->hitBox.canBeTeleported) DrawRectangleRec(collision->area, teleportingColor);
                    else if (this->hitBox.canBounce && other->hitBox.canBeBounced || other->hitBox.canBounce && this->hitBox.canBeBounced) DrawRectangleRec(collision->area, bouncingColor);
                    else DrawRectangleRec(collision->area, (Color){255, 180, 0, 255});
                }
            }
        }
    }
    if (IsKeyDown(KEY_SPACE)) showcollision = true; else showcollision = false;
}

static void React(Entity *e) {
    int othersCount = LAST_ID - e->id - 1;
    if (othersCount == 0) return; // all collisions have been resolved

    Entity *BLOCKING_COLLISIONS[othersCount];
    Entity *PUSHING_COLLISIONS[othersCount];

    Entity *this = e, *other = NULL, *dominantBlockOther = NULL, *dominantPushOther = NULL;
    
    for (int i = e->id; i < LAST_ID; i++)
    {
        if (i == e->id) continue;

        
        other = ENTITY_RECORD[i];
        if (other == NULL) continue;
        
        // NOTE: This condition could mean that if this collision is resolved by position changing, then the next collision resolution won't be triggered
        Collision *collision = GetCollision(this->id, other->id);

        if (CheckCollisionRecs(this->hitBox.area, other->hitBox.area))
        {
            if (this->hitBox.canBlock && other->hitBox.canBeBlocked || other->hitBox.canBlock && this->hitBox.canBeBlocked) {
                if (dominantBlockOther == NULL) dominantBlockOther = other;
                else 
                {
                    Rectangle dominantCollisionArea = GetCollision(this->id, dominantBlockOther->id)->area;
                    Rectangle otherCollisionArea = collision->area;

                    float dominantCollisionAreaSize = dominantCollisionArea.width * dominantCollisionArea.height;
                    float otherCollisionAreaSize = otherCollisionArea.width * otherCollisionArea.height;

                    if (otherCollisionAreaSize > dominantCollisionAreaSize) dominantBlockOther = other;
                }
                
            }

            if (this->hitBox.canPush && other->hitBox.canBePushed || other->hitBox.canPush && this->hitBox.canBePushed) {
                if (dominantPushOther == NULL) dominantPushOther = other;
                else 
                {
                    Rectangle dominantCollisionArea = GetCollision(this->id, dominantPushOther->id)->area;
                    Rectangle otherCollisionArea = collision->area;

                    float dominantCollisionAreaSize = dominantCollisionArea.width * dominantCollisionArea.height;
                    float otherCollisionAreaSize = otherCollisionArea.width * otherCollisionArea.height;

                    if (otherCollisionAreaSize > dominantCollisionAreaSize) dominantPushOther = other;
                }
            }
        }
    }

    if (dominantBlockOther != NULL && dominantBlockOther->hitBox.canBlock && this->hitBox.canBeBlocked) BlockReaction(dominantBlockOther, this, this->SetDestination);
    else if (dominantBlockOther != NULL && dominantBlockOther->hitBox.canBeBlocked && this->hitBox.canBlock) BlockReaction(this, dominantBlockOther, dominantBlockOther->SetDestination);
    

}

static void Free(Entity *e)
{
    int id = e->id;
    
    free(e);
    
    ENTITY_RECORD[id] = NULL;
}

/************************
 * Helper Funcs
 ************************/

static bool IsMoving(Entity *e)
{
    return e->velocity.x != 0 || e->velocity.y != 0;
}

static void SetDestination(Entity *e, float x, float y) {}

static void PushReaction(Entity *pusher, Entity *pushed, void (*setDestination)(Entity *pushed, float x, float y)) {
    
}

static void BlockReaction(Entity *blocker, Entity *blocked, void (*setDestination)(Entity *blocked, float x, float y)) {
    Rectangle blockedHitBox = blocked->hitBox.area;
    Rectangle blockerHitBox = blocker->hitBox.area;
    Rectangle collision = GetCollisionRec(blockerHitBox, blockedHitBox);


    DIRECTIONS blockedDirection = AngleToDirection(blocked->angle);

    switch (blockedDirection)
    {
        case UP:
            setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y + collision.height);
            break;
        case DOWN:
            setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y - collision.height);
            break;
        case LEFT:
            setDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y);
            break;
        case RIGHT:
            setDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y);
            break;
        case UP_LEFT:
            if (collision.width > collision.height) setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y + collision.height);
            else setDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y);
            break;
        case UP_RIGHT:
            if (collision.width > collision.height) setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y + collision.height);
            else setDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y);
            break;
        case DOWN_LEFT:
            if (collision.width > collision.height) setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y - collision.height);
            else setDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y);
            break;
        case DOWN_RIGHT:
            if (collision.width > collision.height) setDestination(blocked, blocked->destFrame.x, blocked->destFrame.y - collision.height);
            else setDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y);
            break;
    } 
}