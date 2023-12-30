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

static void PushReaction(Entity *e1, Entity *e2);
static void BlockReaction(Entity *e1, Entity *e2);

Entity *CreateEntity() 
{
    Entity *e = malloc(sizeof(Entity));
    e->id = LAST_ID++;
    e->child = NULL;
    e->active = true;
    e->extensionCount = 0;
    e->extensions = NULL;
    
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
    e->hitBox.type = HITBOX_RECTANGLE;
    
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

static void Update(Entity *e) {
    e->velocity = (Vector2){0, 0};
    // Vector2 dest = CalculateDestination((Vector2) {e->destFrame.x, e->destFrame.y}, e->velocity, e->angle);
    // SetDestination(e, dest.x, dest.y);
}

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

        if (CheckCollisionRecs(this->hitBox.area, other->hitBox.area))
        {
            if (this->hitBox.canBlock && other->hitBox.canBeBlocked || other->hitBox.canBlock && this->hitBox.canBeBlocked) {
                if (dominantBlockOther == NULL) dominantBlockOther = other;
                else 
                {
                    Rectangle dominantCollisionArea = GetCollisionRec(this->hitBox.area, dominantBlockOther->hitBox.area);
                    Rectangle otherCollisionArea = GetCollisionRec(this->hitBox.area, other->hitBox.area);

                    float dominantCollisionAreaSize = dominantCollisionArea.width * dominantCollisionArea.height;
                    float otherCollisionAreaSize = otherCollisionArea.width * otherCollisionArea.height;

                    if (otherCollisionAreaSize > dominantCollisionAreaSize) dominantBlockOther = other;
                }
                
            }

            if (this->hitBox.canPush && other->hitBox.canBePushed || other->hitBox.canPush && this->hitBox.canBePushed) {
                if (dominantPushOther == NULL) dominantPushOther = other;
                else 
                {
                    Rectangle dominantCollisionArea = GetCollisionRec(this->hitBox.area, dominantPushOther->hitBox.area);
                    Rectangle otherCollisionArea = GetCollisionRec(this->hitBox.area, other->hitBox.area);

                    float dominantCollisionAreaSize = dominantCollisionArea.width * dominantCollisionArea.height;
                    float otherCollisionAreaSize = otherCollisionArea.width * otherCollisionArea.height;

                    if (otherCollisionAreaSize > dominantCollisionAreaSize) dominantPushOther = other;
                }
            }
        }
    }

    // the order of the reactions is important for the entities that can react in multiple ways
    if (dominantPushOther != NULL) PushReaction(this, dominantPushOther);
    if (dominantBlockOther != NULL) BlockReaction(this, dominantBlockOther);
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

static void PushReaction(Entity *e1, Entity *e2) {
    Entity *pushed, *pusher;

    if (e1->hitBox.canPush && e2->hitBox.canBePushed && e1->IsMoving(e1)) {
        pushed = e2;
        pusher = e1;
    } else if (e2->hitBox.canPush && e1->hitBox.canBePushed && e2->IsMoving(e2)) {
        pushed = e1;
        pusher = e2;
    } else return;

    Rectangle pushedHitBox = pushed->hitBox.area;
    Rectangle pusherHitBox = pusher->hitBox.area;
    
    if (!CheckCollisionRecs(pusherHitBox, pushedHitBox)) return;
    
    Rectangle collision = GetCollisionRec(pusherHitBox, pushedHitBox);

    DIRECTIONS pusherDirection = AngleToDirection(pusher->angle);
    DIRECTIONS collisionDirection = DetectCollisionDirection(pusher->id, pushed->id);
    DIRECTIONS pushDirection;

    if (collisionDirection == UP && (pusherDirection == DOWN || pusherDirection == DOWN_LEFT || pusherDirection == DOWN_RIGHT)) pushDirection = DOWN;
    if (collisionDirection == DOWN && (pusherDirection == UP || pusherDirection == UP_LEFT || pusherDirection == UP_RIGHT)) pushDirection = UP;
    if (collisionDirection == LEFT && (pusherDirection == RIGHT || pusherDirection == UP_RIGHT || pusherDirection == DOWN_RIGHT)) pushDirection = RIGHT;
    if (collisionDirection == RIGHT && (pusherDirection == LEFT || pusherDirection == UP_LEFT || pusherDirection == DOWN_LEFT)) pushDirection = LEFT;
    if (collisionDirection == UP_LEFT && (pusherDirection == DOWN_RIGHT || pusherDirection == RIGHT || pusherDirection == DOWN)) pushDirection = DOWN_RIGHT;
    if (collisionDirection == UP_RIGHT && (pusherDirection == DOWN_LEFT || pusherDirection == LEFT || pusherDirection == DOWN)) pushDirection = DOWN_LEFT;
    if (collisionDirection == DOWN_LEFT && (pusherDirection == UP_RIGHT || pusherDirection == RIGHT || pusherDirection == UP)) pushDirection = UP_RIGHT;
    if (collisionDirection == DOWN_RIGHT && (pusherDirection == UP_LEFT || pusherDirection == LEFT || pusherDirection == UP)) pushDirection = UP_LEFT;

    static int counter = 0;
    if (counter++ % 20 == 0) printf("Entity %d is pushing entity %d with velocity (%f, %f) to the %s\n", pusher->id, pushed->id, pusher->velocity.x, pusher->velocity.y, DirectionToString(AngleToDirection(pusher->angle)));
    

    Vector2 pusherDest = (Vector2){pusher->destFrame.x, pusher->destFrame.y};
    Vector2 pushedDest = (Vector2){pushed->destFrame.x, pushed->destFrame.y};

    float angle = pushed->angle = DirectionToAngle(pushDirection);
    Vector2 pushedVelocity = pushed->velocity = (Vector2){pusher->velocity.x, pusher->velocity.y};
    Vector2 pusherVelocity = (Vector2){1 * pusher->velocity.x, 1 * pusher->velocity.y};

    pushedDest = CalculateDestination(pushedDest, pushedVelocity, angle);
    pusherDest = CalculateDestination(pusherDest, pusherVelocity, angle + PI);
    
    pushed->SetDestination(pushed, pushedDest.x, pushedDest.y);
    pusher->SetDestination(pusher, pusherDest.x, pusherDest.y);
}

static void BlockReaction(Entity *e1, Entity *e2) {
    Entity *blocker, *blocked;

    if (e1->hitBox.canBlock && e2->IsMoving(e2) && e2->hitBox.canBeBlocked) {
        blocker = e1;
        blocked = e2;
    } else if (e2->hitBox.canBlock && e1->IsMoving(e1) && e1->hitBox.canBeBlocked) {
        blocker = e2;
        blocked = e1;
    } else return;

    Rectangle blockedHitBox = blocked->hitBox.area;
    Rectangle blockerHitBox = blocker->hitBox.area;

    if (!CheckCollisionRecs(blockerHitBox, blockedHitBox)) return;
    
    Rectangle collision = GetCollisionRec(blockerHitBox, blockedHitBox);

    DIRECTIONS blockedDirection = AngleToDirection(blocked->angle);
    DIRECTIONS collisionDirection = DetectCollisionDirection(blocked->id, blocker->id);
    DIRECTIONS blockDirection;

    if (collisionDirection == UP && (blockedDirection == DOWN || blockedDirection == DOWN_LEFT || blockedDirection == DOWN_RIGHT)) blockDirection = UP;
    if (collisionDirection == DOWN && (blockedDirection == UP || blockedDirection == UP_LEFT || blockedDirection == UP_RIGHT)) blockDirection = DOWN;
    if (collisionDirection == LEFT && (blockedDirection == RIGHT || blockedDirection == UP_RIGHT || blockedDirection == DOWN_RIGHT)) blockDirection = LEFT;
    if (collisionDirection == RIGHT && (blockedDirection == LEFT || blockedDirection == UP_LEFT || blockedDirection == DOWN_LEFT)) blockDirection = RIGHT;
    if (collisionDirection == UP_LEFT && (blockedDirection == DOWN_RIGHT || blockedDirection == RIGHT || blockedDirection == DOWN)) blockDirection = UP_LEFT;
    if (collisionDirection == UP_RIGHT && (blockedDirection == DOWN_LEFT || blockedDirection == LEFT || blockedDirection == DOWN)) blockDirection = UP_RIGHT;
    if (collisionDirection == DOWN_LEFT && (blockedDirection == UP_RIGHT || blockedDirection == RIGHT || blockedDirection == UP)) blockDirection = DOWN_LEFT;
    if (collisionDirection == DOWN_RIGHT && (blockedDirection == UP_LEFT || blockedDirection == LEFT || blockedDirection == UP)) blockDirection = DOWN_RIGHT;

    if (blockDirection == UP) blocked->SetDestination(blocked, blocked->destFrame.x, blocked->destFrame.y - collision.height);
    if (blockDirection == DOWN) blocked->SetDestination(blocked, blocked->destFrame.x, blocked->destFrame.y + collision.height);
    if (blockDirection == LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y);
    if (blockDirection == RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y);
    if (blockDirection == UP_LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y - collision.height);
    if (blockDirection == UP_RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y - collision.height);
    if (blockDirection == DOWN_LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y + collision.height);
    if (blockDirection == DOWN_RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y + collision.height);
 
}