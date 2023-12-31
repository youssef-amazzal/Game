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
static void BounceReaction(Entity *e1, Entity *e2);

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
    if (IsKeyDown(KEY_SPACE)) showcollision = true; else showcollision = false;
    
    if (showcollision) {
        DrawRectangleLinesEx(e->destFrame, 1, (Color){255, 255, 255, 80});
        
        
        DrawCircleLines(e->hitBox.circle.center.x, e->hitBox.circle.center.y, e->hitBox.circle.radius, e->hitBox.color);
        DrawRectangleLinesEx(e->hitBox.area, 1, e->hitBox.color);
        
        int fontSize = 10;
        Color textColor = WHITE;
        if (e->zIndex == 0) DrawText(TextFormat("%d", e->id), e->destFrame.x + fontSize/2, e->destFrame.y + fontSize/2, fontSize, textColor);
        
        if (e->hitBox.type == HITBOX_CIRCLE) return;

        for (int i = 0; i < e->id; i++) {
            
            Entity *this = e;
            Entity *other = ENTITY_RECORD[i];

            if (other->hitBox.type == HITBOX_CIRCLE) continue; 

            if (other != NULL) {
                Rectangle collision = GetCollisionRec(this->hitBox.area, other->hitBox.area);
                Rectangle collision2 = GetCollisionRec(this->destFrame, other->hitBox.area);

                Color collisionColor = (Color){255, 180, 0, 120};
                Color blockingColor = (Color){255, 0, 0, 120};
                Color pushingColor = (Color){0, 255, 0, 120};
                Color destroyingColor = (Color){0, 0, 255, 120};
                Color collectingColor = (Color){255, 0, 255, 120};
                Color teleportingColor = (Color){0, 255, 255, 120};
                Color bouncingColor = (Color){255, 255, 0, 120};

                if (collision.width > 0 && collision.height > 0) {
                    if (this->hitBox.canBlock && other->hitBox.canBeBlocked || other->hitBox.canBlock && this->hitBox.canBeBlocked) DrawRectangleRec(collision, blockingColor);
                    else if (this->hitBox.canPush && other->hitBox.canBePushed || other->hitBox.canPush && this->hitBox.canBePushed) DrawRectangleRec(collision, pushingColor);
                    else if (this->hitBox.canDestroy && other->hitBox.canBeDestroyed || other->hitBox.canDestroy && this->hitBox.canBeDestroyed) DrawRectangleRec(collision, destroyingColor);
                    else if (this->hitBox.canCollect && other->hitBox.canBeCollected || other->hitBox.canCollect && this->hitBox.canBeCollected) DrawRectangleRec(collision, collectingColor);
                    else if (this->hitBox.canTeleport && other->hitBox.canBeTeleported || other->hitBox.canTeleport && this->hitBox.canBeTeleported) DrawRectangleRec(collision, teleportingColor);
                    else if (this->hitBox.canBounce && other->hitBox.canBeBounced || other->hitBox.canBounce && this->hitBox.canBeBounced) DrawRectangleRec(collision, bouncingColor);
                    else DrawRectangleRec(collision, (Color){255, 180, 0, 255});

                    

                }
            }
        }
    }
}

static void React(Entity *e) {

    Entity *this = e, *other = NULL;

    for (int i = 0; i < LAST_ID; i++)
    {
        if (i == e->id) continue;
        other = ENTITY_RECORD[i];

        if (this == NULL || other == NULL) continue;
        if (!CheckCollision(this->id, other->id)) continue;
        
        PushReaction(this, other);
        BlockReaction(this, other);
        BounceReaction(this, other);
    }
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

    if (pusher->hitBox.type == HITBOX_CIRCLE || pushed->hitBox.type == HITBOX_CIRCLE) return; // TODO: until i implement collision for non-rectangle shapes

    Rectangle pushedHitBox = pushed->hitBox.area;
    Rectangle pusherHitBox = pusher->hitBox.area;

    Rectangle collision = GetCollisionRec(pusherHitBox, pushedHitBox);

    DIRECTIONS pusherDirection = AngleToDirection(pusher->angle);
    DIRECTIONS collisionDirection = DetectCollisionDirection(pusher->id, pushed->id);
    DIRECTIONS pushDirection = -1;

    if (collisionDirection == UP && (pusherDirection == DOWN || pusherDirection == DOWN_LEFT || pusherDirection == DOWN_RIGHT)) pushDirection = DOWN;
    if (collisionDirection == DOWN && (pusherDirection == UP || pusherDirection == UP_LEFT || pusherDirection == UP_RIGHT)) pushDirection = UP;
    if (collisionDirection == LEFT && (pusherDirection == RIGHT || pusherDirection == UP_RIGHT || pusherDirection == DOWN_RIGHT)) pushDirection = RIGHT;
    if (collisionDirection == RIGHT && (pusherDirection == LEFT || pusherDirection == UP_LEFT || pusherDirection == DOWN_LEFT)) pushDirection = LEFT;
    if (collisionDirection == UP_LEFT && (pusherDirection == DOWN_RIGHT || pusherDirection == RIGHT || pusherDirection == DOWN)) pushDirection = DOWN_RIGHT;
    if (collisionDirection == UP_RIGHT && (pusherDirection == DOWN_LEFT || pusherDirection == LEFT || pusherDirection == DOWN)) pushDirection = DOWN_LEFT;
    if (collisionDirection == DOWN_LEFT && (pusherDirection == UP_RIGHT || pusherDirection == RIGHT || pusherDirection == UP)) pushDirection = UP_RIGHT;
    if (collisionDirection == DOWN_RIGHT && (pusherDirection == UP_LEFT || pusherDirection == LEFT || pusherDirection == UP)) pushDirection = UP_LEFT;

    if (pushDirection == -1) return;
    // if (collisionDirection == UP) pushDirection = DOWN;
    // if (collisionDirection == DOWN) pushDirection = UP;
    // if (collisionDirection == LEFT) pushDirection = RIGHT;
    // if (collisionDirection == RIGHT) pushDirection = LEFT;
    // if (collisionDirection == UP_LEFT) pushDirection = DOWN_RIGHT;
    // if (collisionDirection == UP_RIGHT) pushDirection = DOWN_LEFT;
    // if (collisionDirection == DOWN_LEFT) pushDirection = UP_RIGHT;
    // if (collisionDirection == DOWN_RIGHT) pushDirection = UP_LEFT;

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
    if (e1->hitBox.canPush && e2->hitBox.canBePushed || e2->hitBox.canPush && e1->hitBox.canBePushed) return; // pushable reactions are handled in PushReaction 
    
    
    Entity *blocker, *blocked;

    if (e1->hitBox.canBlock && e2->hitBox.canBeBlocked && !e1->IsMoving(e1)) {
        blocker = e1;
        blocked = e2;
    } else return;

    if (blocker->hitBox.type == HITBOX_CIRCLE || blocked->hitBox.type == HITBOX_CIRCLE) return; // TODO: until i implement collision for non-rectangle shapes

    Rectangle blockedHitBox = blocked->hitBox.area;
    Rectangle blockerHitBox = blocker->hitBox.area;
    
    Rectangle collision = GetCollisionRec(blockerHitBox, blockedHitBox);

    DIRECTIONS blockedDirection = AngleToDirection(blocked->angle);
    DIRECTIONS collisionDirection = DetectCollisionDirection(blocked->id, blocker->id);
    DIRECTIONS blockDirection;

    // if (collisionDirection == UP && (blockedDirection == DOWN || blockedDirection == DOWN_LEFT || blockedDirection == DOWN_RIGHT)) blockDirection = UP;
    // if (collisionDirection == DOWN && (blockedDirection == UP || blockedDirection == UP_LEFT || blockedDirection == UP_RIGHT)) blockDirection = DOWN;
    // if (collisionDirection == LEFT && (blockedDirection == RIGHT || blockedDirection == UP_RIGHT || blockedDirection == DOWN_RIGHT)) blockDirection = LEFT;
    // if (collisionDirection == RIGHT && (blockedDirection == LEFT || blockedDirection == UP_LEFT || blockedDirection == DOWN_LEFT)) blockDirection = RIGHT;
    // if (collisionDirection == UP_LEFT && (blockedDirection == DOWN_RIGHT || blockedDirection == RIGHT || blockedDirection == DOWN)) blockDirection = UP_LEFT;
    // if (collisionDirection == UP_RIGHT && (blockedDirection == DOWN_LEFT || blockedDirection == LEFT || blockedDirection == DOWN)) blockDirection = UP_RIGHT;
    // if (collisionDirection == DOWN_LEFT && (blockedDirection == UP_RIGHT || blockedDirection == RIGHT || blockedDirection == UP)) blockDirection = DOWN_LEFT;
    // if (collisionDirection == DOWN_RIGHT && (blockedDirection == UP_LEFT || blockedDirection == LEFT || blockedDirection == UP)) blockDirection = DOWN_RIGHT;

    if (collisionDirection == UP) blocked->SetDestination(blocked, blocked->destFrame.x, blocked->destFrame.y - collision.height);
    if (collisionDirection == DOWN) blocked->SetDestination(blocked, blocked->destFrame.x, blocked->destFrame.y + collision.height);
    if (collisionDirection == LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y);
    if (collisionDirection == RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y);
    if (collisionDirection == UP_LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y - collision.height);
    if (collisionDirection == UP_RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y - collision.height);
    if (collisionDirection == DOWN_LEFT) blocked->SetDestination(blocked, blocked->destFrame.x - collision.width, blocked->destFrame.y + collision.height);
    if (collisionDirection == DOWN_RIGHT) blocked->SetDestination(blocked, blocked->destFrame.x + collision.width, blocked->destFrame.y + collision.height);
 
}

static void BounceReaction(Entity *e1, Entity *e2) {
    Entity *bouncer, *bounced;

    if (e1->hitBox.canBounce && e2->hitBox.canBeBounced) {
        bouncer = e1; // The one that is collided into
        bounced = e2; // The moving one that should bounce
    } else if (e2->hitBox.canBounce && e1->hitBox.canBeBounced) {
        bouncer = e2;
        bounced = e1;
    } else return;

    bool bouncerIsCircle = bouncer->hitBox.type == HITBOX_CIRCLE;
    bool bouncedIsCircle = bounced->hitBox.type == HITBOX_CIRCLE;

    Rectangle collision = GetCollisionRec(bounced->destFrame, bouncer->hitBox.area);
    DIRECTIONS collisionDirection = DetectCollisionDirection(bounced->id, bouncer->id);

    if (bouncedIsCircle && !bouncerIsCircle) {
        if (collisionDirection == UP) bounced->SetDestination(bounced, bounced->destFrame.x, bounced->destFrame.y - collision.height);
        if (collisionDirection == DOWN) bounced->SetDestination(bounced, bounced->destFrame.x, bounced->destFrame.y + collision.height);
        if (collisionDirection == LEFT) bounced->SetDestination(bounced, bounced->destFrame.x - collision.width, bounced->destFrame.y);
        if (collisionDirection == RIGHT) bounced->SetDestination(bounced, bounced->destFrame.x + collision.width, bounced->destFrame.y);
        if (collisionDirection == UP_LEFT) bounced->SetDestination(bounced, bounced->destFrame.x - collision.width, bounced->destFrame.y - collision.height);
        if (collisionDirection == UP_RIGHT) bounced->SetDestination(bounced, bounced->destFrame.x + collision.width, bounced->destFrame.y - collision.height);
        if (collisionDirection == DOWN_LEFT) bounced->SetDestination(bounced, bounced->destFrame.x - collision.width, bounced->destFrame.y + collision.height);
        

        bounced->angle += 90;
    }
    
    
}
/************************
 * Public Funcs
 ************************/

void SetTexturesData(Entity *e, TextureData textureData) {

    HitBox hitBox = e->hitBox;
    hitBox.type = textureData.hitBox.type;
    
    if (textureData.hitBox.type == HITBOX_CIRCLE) e->hitBox.circle.radius = textureData.hitBox.circle.radius * SCALING_FACTOR;
    if (textureData.hitBox.type == HITBOX_RECTANGLE) {
        hitBox.area.width = textureData.hitBox.area.width * SCALING_FACTOR;
        hitBox.area.height = textureData.hitBox.area.height * SCALING_FACTOR;
    }

    e->hitBox.color = textureData.hitBox.color;
    e->hitBox.isDisabled = textureData.hitBox.isDisabled;
    e->zIndex = textureData.zIndex;

    e->hitBox.canBlock = textureData.hitBox.canBlock;
    e->hitBox.canBeBlocked = textureData.hitBox.canBeBlocked;

    e->hitBox.canPush = textureData.hitBox.canPush;
    e->hitBox.canBePushed = textureData.hitBox.canBePushed;

    e->hitBox.canDestroy = textureData.hitBox.canDestroy;
    e->hitBox.canBeDestroyed = textureData.hitBox.canBeDestroyed;

    e->hitBox.canCollect = textureData.hitBox.canCollect;
    e->hitBox.canBeCollected = textureData.hitBox.canBeCollected;

    e->hitBox.canBounce = textureData.hitBox.canBounce;
    e->hitBox.canBeBounced = textureData.hitBox.canBeBounced;

    e->hitBox.canTeleport = textureData.hitBox.canTeleport;
    e->hitBox.canBeTeleported = textureData.hitBox.canBeTeleported;
}

void MoveHitBox(int id, TextureData textureData)
{
    Entity *entity = ENTITY_RECORD[id];
    switch(entity->hitBox.type) {
        case HITBOX_RECTANGLE:
            entity->hitBox.area.x = textureData.hitBox.area.x * SCALING_FACTOR + entity->destFrame.x;
            entity->hitBox.area.y = textureData.hitBox.area.y * SCALING_FACTOR + entity->destFrame.y;
            break;
        case HITBOX_CIRCLE:
            entity->hitBox.circle.center.x = textureData.hitBox.circle.center.x * SCALING_FACTOR + entity->destFrame.x;
            entity->hitBox.circle.center.y = textureData.hitBox.circle.center.y * SCALING_FACTOR + entity->destFrame.y;
            break;
    }
}
