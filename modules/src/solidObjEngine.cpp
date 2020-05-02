#include "solidObjEngine.h"

static olc::vi2d calculate2Ddistance(olc::vi2d p1, olc::vi2d p2)
{
    int distance = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
    return {distance, distance}; 
}

SolidObject::SolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius)
{
    this->position       = onCreatePosition;
    this->radius         = onCreateRadius;
    this->gameEngine     = solidEngine;
    this->collisionSpace = INVALID_COLLISION_HANDLE;
}

olc::vi2d SolidObject::updatePosition(olc::vi2d& destination, void* Args)
{
    olc::vi2d resultDestination = this->objectMovementFunction(Args);
    resultDestination           = this->gameEngine->calculateSolidDestination(this,resultDestination);

    return resultDestination;
}

olc::vi2d SolidObject::getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    olc::vi2d allowedDestination = calculate2Ddistance(this->position,targetDestination);

    (targetObject->position.x < this->position.x) ? allowedDestination.x -= (this->radius+targetObject->radius) : allowedDestination.x += (this->radius+targetObject->radius);
    (targetObject->position.y < this->position.y) ? allowedDestination.y -= (this->radius+targetObject->radius) : allowedDestination.y += (this->radius+targetObject->radius);
    
    return allowedDestination;
}

void SolidObjGameEngine::registerSolidObject(CollisionSpaceHandle_t collisionSpace,SolidObject* objectPtr)
{
    if(collisionSpace < this->allocatedCollisionSpaces)
    {
        this->collisionMatrix[collisionSpace]->collisionsArray[this->collisionMatrix[collisionSpace]->registeredCollisions++] = objectPtr;
        objectPtr->setCollisionSpace(collisionSpace);
    }
}

CollisionSpaceHandle_t SolidObjGameEngine::createCollisionSpace(olc::vi2d* collisionSurface, uint32_t size)
{
    CollisionSurface_t* toInsertCollisionSurface     = new CollisionSurface_t;

    toInsertCollisionSurface->collisionSurfaceSize   = size;
    toInsertCollisionSurface->registeredCollisions   = 0;

    // for(int i = 0; i < size; i++)
    // {
    //     //toInsertCollisionSurface[i].collisionSurfacePoints = new CollisionSurfaceCorners_t;
    //     toInsertCollisionSurface[i].collisionSurfacePoints[i] = collisionSurface[i];
    // }

    this->collisionMatrix[allocatedCollisionSpaces] = toInsertCollisionSurface;

    this->allocatedCollisionSpaces++;
    return (this->allocatedCollisionSpaces-1);
}

olc::vi2d SolidObjGameEngine::calculateSolidDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    if(targetObject->getCollisionSpace() == INVALID_COLLISION_HANDLE) return targetDestination;

    CollisionSurface_t* targetSurface = this->collisionMatrix[targetObject->getCollisionSpace()];
    CollisionVector_t   collisionsArray = targetSurface->collisionsArray;

    for(auto registeredObject = collisionsArray.begin(); registeredObject != collisionsArray.end(); registeredObject++)
    {
        if((*registeredObject) != targetObject)
        {
            targetDestination = (*registeredObject)->getAllowedDestination(targetObject, targetDestination);
        }
    }

    return targetDestination;
}