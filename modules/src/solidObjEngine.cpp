#include "solidObjEngine.h"
#include <cmath>

//******************************** local static functions ***************//
static olc::vi2d calculate2Ddistance(olc::vi2d p1, olc::vi2d p2)
{
    olc::vi2d distance;
    distance.x = abs(p1.x-p2.x);
    distance.y = abs(p1.y-p2.y);
    return distance; 
}

static bool pnpolySurfaceBoundsCalculation(int size, CollisionSurfaceCorners_t& surface, olc::vi2d point)
{ 
  bool intersect = false;
  int j  = 0;
  for (int i = 0, j = size-1; i < size; j = i++) {
    if ( ((surface[i].y>point.y) != (surface[j].y>point.y)) &&
     (point.x < (surface[j].x-surface[i].x) * (point.y-surface[i].y) / (surface[j].y-surface[i].y) + surface[i].x) )
       intersect = !intersect;
  }
  return intersect;
}

//******************************** SolidObject Base Class ***************//

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

//******************************** SolidRoundObject *********************//
olc::vi2d SolidRoundObject::getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{   
    //target object variable preparation
    olc::vi2d targetObjectPosition = targetObject->getPosition(); 
    olc::vi2d targetObjectDistance;
    targetObjectDistance.x = targetDestination.x - targetObjectPosition.x;
    targetObjectDistance.y = targetDestination.y - targetObjectPosition.y;

    olc::vi2d objsDistance         = calculate2Ddistance(this->position,targetDestination);
    if(std::sqrt(std::pow(objsDistance.x,2)+std::pow(objsDistance.y,2)) > (this->getRadius()+targetObject->getRadius()))
        return targetObjectDistance;

    //distance calculation variable preparation
    olc::vi2d allowedDestination   = objsDistance;
    unsigned int slope = (allowedDestination.x == 0) ? allowedDestination.y:allowedDestination.y/allowedDestination.x;
    float yCorrection  = 0;
    float xCorrection  = 0;

    if(allowedDestination.x != 0)
    {
        xCorrection = std::cos(atan(slope))*(this->getRadius())+std::cos(atan(slope))*(targetObject->getRadius());
        (xCorrection <= allowedDestination.x) ? allowedDestination.x -= std::round(xCorrection): allowedDestination.x = 0;
    }
    if(allowedDestination.y != 0)
    {
        yCorrection = std::sin(atan(slope))*(this->getRadius())+std::sin(atan(slope))*(targetObject->getRadius());
        (yCorrection <= allowedDestination.y) ? allowedDestination.y -= std::round(yCorrection): allowedDestination.y = 0;
        
    }

    if(allowedDestination.x != 0 && targetDestination.x < allowedDestination.x)
        allowedDestination.x = targetDestination.x;
    if(allowedDestination.y != 0 && targetDestination.y < allowedDestination.y)
        allowedDestination.y = targetDestination.y;   

    return allowedDestination;
}

//******************************** SolidPolygonObject ********************//

olc::vi2d SolidPolygonObject::getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    //target object variable preparation
    olc::vi2d targetObjectPosition = targetObject->getPosition(); 
    olc::vi2d targetObjectDistance;
    targetObjectDistance.x = targetDestination.x - targetObjectPosition.x;
    targetObjectDistance.y = targetDestination.y - targetObjectPosition.y;
    olc::vi2d targetDirection;
    targetObjectDistance.x < 0? targetDirection.x = -targetObject->getRadius():targetObjectDistance.x==0 ? targetDirection.x = 0: targetDirection.x = targetObject->getRadius(); 
    targetObjectDistance.y < 0? targetDirection.y = -targetObject->getRadius():targetObjectDistance.y==0 ? targetDirection.y = 0: targetDirection.y = targetObject->getRadius(); 

    if(!pnpolySurfaceBoundsCalculation(this->surfaceSize,this->surfacecorners,targetDestination+targetDirection))
        return targetObjectDistance;

    if(!pnpolySurfaceBoundsCalculation(this->surfaceSize,this->surfacecorners,olc::vi2d(targetDestination.x+targetDirection.x,0)))
        targetObjectDistance.x = 0;

    if(!pnpolySurfaceBoundsCalculation(this->surfaceSize,this->surfacecorners,olc::vi2d(0,targetDestination.y+targetDirection.y)))
        targetObjectDistance.y = 0;

    return targetObjectDistance;
}

//******************************** SolidPolygonObject ********************//

olc::vi2d SolidLineObject::getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    //target object variable preparation
    olc::vi2d targetObjectPosition = targetObject->getPosition(); 
    olc::vi2d targetObjectDistance;
    targetObjectDistance.x = targetDestination.x - targetObjectPosition.x;
    targetObjectDistance.y = targetDestination.y - targetObjectPosition.y;
    olc::vi2d targetDirection;
    targetObjectDistance.x < 0? targetDirection.x = -targetObject->getRadius():targetObjectDistance.x==0 ? targetDirection.x = 0: targetDirection.x = targetObject->getRadius(); 
    targetObjectDistance.y < 0? targetDirection.y = -targetObject->getRadius():targetObjectDistance.y==0 ? targetDirection.y = 0: targetDirection.y = targetObject->getRadius(); 

    olc::vi2d centerToTargetDistanceVector = calculate2Ddistance(this->getPosition(),targetObject->getPosition());
    unsigned int centerToTargetDistance = std::sqrt(std::pow(centerToTargetDistanceVector.x,2)+std::pow(centerToTargetDistanceVector.y,2));

    unsigned int centerOnLineOffset = std::sqrt(std::pow(centerToTargetDistance,2)-std::pow(targetObject->getRadius(),2));

    int xAxisDistance   = this->surfacecorners[0].x - this->surfacecorners[1].x;
    int Slope = (xAxisDistance == 0) ? Slope = (this->surfacecorners[0].y-this->surfacecorners[1].y):Slope = ((this->surfacecorners[0].y-this->surfacecorners[1].y)/xAxisDistance);

    olc::vi2d intersection;
    if(Slope != 0)
    {
        intersection.x = Slope;
        intersection.y = Slope;
    }
    else
    {
        intersection.x = Slope;
        intersection.y = 0;
    }

    olc::vi2d intersectionPointOnLine = this->getPosition()+(intersection*centerOnLineOffset);

    olc::vi2d intersectionToCenterDistance = calculate2Ddistance(intersectionPointOnLine,targetDestination);
    unsigned int intersectionToCenterDistanceModule = 
        std::sqrt(std::pow(intersectionToCenterDistance.x,2)+std::pow(intersectionToCenterDistance.y,2));

    if(intersectionToCenterDistanceModule > targetObject->getRadius())
        return targetObjectDistance;

    olc::vi2d xIntersectionToCenterDistance = calculate2Ddistance(intersectionPointOnLine,olc::vi2d(targetDestination.x+targetDirection.x,0));
    unsigned int xIntersectionToCenterDistanceModule = 
        std::sqrt(std::pow(xIntersectionToCenterDistance.x,2)+std::pow(xIntersectionToCenterDistance.y,2));
    if(xIntersectionToCenterDistanceModule > targetObject->getRadius())
        targetObjectDistance.x = 0;

    olc::vi2d yIntersectionToCenterDistance = calculate2Ddistance(intersectionPointOnLine,olc::vi2d(0,targetDestination.y+targetDirection.y));
    unsigned int yIntersectionToCenterDistanceModule = 
        std::sqrt(std::pow(yIntersectionToCenterDistance.x,2)+std::pow(yIntersectionToCenterDistance.y,2));
    if(yIntersectionToCenterDistanceModule > targetObject->getRadius())
        targetObjectDistance.y = 0;

    return targetObjectDistance;
}

//******************************** SolidObjGameEngine *******************//

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
        toInsertCollisionSurface->collisionSurfacePoints.push_back(collisionSurface[i]);
    }

    this->collisionMatrix.push_back(toInsertCollisionSurface);

    this->allocatedCollisionSpaces++;
    return (this->allocatedCollisionSpaces-1);
}

olc::vi2d SolidObjGameEngine::calculateSolidDestination(SolidObject* targetObject, olc::vi2d targetDestination)
{
    if(targetObject->getCollisionSpace() == INVALID_COLLISION_HANDLE) return targetDestination;
    olc::vi2d targetCalculatedDestination = {0,0};
    olc::vi2d targetResultDestination     = targetDestination;

    CollisionSurface_t* targetSurface = this->collisionMatrix[targetObject->getCollisionSpace()];
    CollisionVector_t   collisionsArray = targetSurface->collisionsArray;

    for(auto registeredObject = collisionsArray.begin(); registeredObject != collisionsArray.end(); registeredObject++)
    {
        if((*registeredObject) != targetObject)
        {
            if(pnpolySurfaceBoundsCalculation(targetSurface->collisionSurfaceSize,targetSurface->collisionSurfacePoints,(*registeredObject)->getPosition()))
            {
                targetCalculatedDestination = (*registeredObject)->getAllowedDestination(targetObject, targetDestination);

                if (abs(targetCalculatedDestination.x) < abs(targetResultDestination.x))
                    targetResultDestination.x = targetCalculatedDestination.x;

                if (abs(targetCalculatedDestination.y) < abs(targetResultDestination.y))
                    targetResultDestination.y = targetCalculatedDestination.y;
            }
            else
            {
                //case current obstacle is not within the surface
                targetResultDestination.x = targetDestination.x - targetObject->getPosition().x;
                targetResultDestination.y = targetDestination.y - targetObject->getPosition().y;
            }
        }
    }

    return targetResultDestination;
}