#include "solidObjEngine.h"

SolidObject::SolidObject(SolidObjGameEngine* solidEngine)
{
    this->gameEngine = solidEngine;
}

// olc::vi2d SolidObject::getCollisionPoint(void)
// {

// }

// olc::vi2d SolidObject::checkCollision(olc::vi2d& destination, unsigned int radius)
// {
//     return {0,0};
// }

void SolidObjGameEngine::registerCollision(CollisionSpaceHandle_t collisionSpace,SolidObject* ObjectPtr)
{
    if(collisionSpace < this->AllocatedCollisionSpaces)
    {
        this->CollisionMatrix[collisionSpace]->collisionsArray[this->CollisionMatrix[collisionSpace]->registeredCollisions++] = ObjectPtr;
    }
}

CollisionSpaceHandle_t SolidObjGameEngine::createCollisionSpace(olc::vi2d* collisionSurface, uint32_t size)
{
    CollisionSurface_t* toInsertCollisionSurface     = new CollisionSurface_t;
    toInsertCollisionSurface->collisionSurfaceSize   = size;
    toInsertCollisionSurface->registeredCollisions   = 0;

    for(int i = 0; i < size; i++)
    {
        toInsertCollisionSurface[i].collisionSurfacePoints[i] = collisionSurface[i];
    }

    this->CollisionMatrix[AllocatedCollisionSpaces++] = toInsertCollisionSurface;

    return (this->AllocatedCollisionSpaces-1);
}