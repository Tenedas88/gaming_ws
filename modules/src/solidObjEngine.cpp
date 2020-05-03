#include "solidObjEngine.h"
#include <cmath>

static olc::vi2d calculate2Ddistance(olc::vi2d p1, olc::vi2d p2)
{
    olc::vi2d distance;
    distance.x = abs(p1.x-p2.x);
    distance.y = abs(p1.y-p2.y);
    return distance; 
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

    this->position.x += resultDestination.x;
    this->position.y += resultDestination.y;
    return this->position;
}

olc::vi2d SolidObject::getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    olc::vi2d targetObjectPosition = targetObject->getPosition(); 
    olc::vi2d targetObjectDistance;
    targetObjectDistance.x = targetDestination.x - targetObjectPosition.x;
    targetObjectDistance.y = targetDestination.y - targetObjectPosition.y;

    olc::vi2d allowedDestination   = calculate2Ddistance(this->position,targetDestination);
    if(std::sqrt(std::pow(allowedDestination.x,2)+std::pow(allowedDestination.y,2)) > (this->getRadius()+targetObject->getRadius()))
        return targetObjectDistance;

    unsigned int slope = (allowedDestination.x == 0) ? allowedDestination.y:allowedDestination.y/allowedDestination.x;
    float yCorrection  = 0;
    float xCorrection  = 0;

    //TODO: check allowed position signes transformations
    if(allowedDestination.x != 0)
    {
        xCorrection = std::cos(atan(slope))*(this->radius)+std::cos(atan(slope))*(targetObject->radius);
        (xCorrection <= allowedDestination.x) ? allowedDestination.x -= std::round(xCorrection): allowedDestination.x = 0;
    }
    if(allowedDestination.y != 0)
    {
        yCorrection = std::sin(atan(slope))*(this->radius)+std::sin(atan(slope))*(targetObject->radius);
        (yCorrection <= allowedDestination.y) ? allowedDestination.y -= std::round(yCorrection): allowedDestination.y = 0;
        
    }

    if((abs(targetDestination.x) - abs(this->position.x)) > (this->getRadius()+targetObject->getRadius())) allowedDestination.x = targetObjectDistance.x;
    if((abs(targetDestination.y) - abs(this->position.y)) > (this->getRadius()+targetObject->getRadius())) allowedDestination.y = targetObjectDistance.y;
    //(abs(targetObjectDistance.x) <= allowedDestination.x) ? allowedDestination.x = targetObjectDistance.x : allowedDestination.x *= (targetDestination.x/targetDestination.x);
    //(abs(targetObjectDistance.y) <= allowedDestination.y) ? allowedDestination.y = targetObjectDistance.y : allowedDestination.y *= (targetDestination.y/targetDestination.y);

    return allowedDestination;
}

void SolidObjGameEngine::registerSolidObject(CollisionSpaceHandle_t collisionSpace,SolidObject* objectPtr)
{
    if(collisionSpace < this->allocatedCollisionSpaces)
    {
        this->collisionMatrix[collisionSpace]->collisionsArray.push_back(objectPtr);
        this->collisionMatrix[collisionSpace]->registeredCollisions++;
        objectPtr->setCollisionSpace(collisionSpace);
    }
}

CollisionSpaceHandle_t SolidObjGameEngine::createCollisionSpace(olc::vi2d* collisionSurface, uint32_t size)
{
    CollisionSurface_t* toInsertCollisionSurface     = new CollisionSurface_t;

    toInsertCollisionSurface->collisionSurfaceSize   = size;
    toInsertCollisionSurface->registeredCollisions   = 0;

    for(int i = 0; i < size; i++)
    {
        toInsertCollisionSurface[i].collisionSurfacePoints.push_back(collisionSurface[i]);
    }

    this->collisionMatrix.push_back(toInsertCollisionSurface);

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