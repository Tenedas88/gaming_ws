#include "olcPixelGameEngine.h"
#include <vector>

class SolidObject;
class SolidObjGameEngine;

//handle of a given collision surface
typedef uint32_t CollisionSpaceHandle_t;
//vector containing the corner points of a collision surface
typedef std::vector<olc::vi2d> CollisionSurfaceCorners_t;
//vector containing the SolidObject Pointers - associated with a surface within CollisionSurface_t
typedef std::vector<SolidObject*> CollisionVector_t;

//data structure to gather collision surface informations
typedef struct _CollisionSurface_t
{
    CollisionSurfaceCorners_t  collisionSurfacePoints;
    uint32_t                   collisionSurfaceSize;
    uint32_t                   registeredCollisions;
    CollisionVector_t          collisionsArray;
} CollisionSurface_t;

//SolidObject is the base object of the SolidObjGameEngine
class SolidObject 
{
    public:
        SolidObject(SolidObjGameEngine* solidEngine);
        // olc::vi2d getCollisionPoint(void);
        // olc::vi2d checkCollision(olc::vi2d& destination, unsigned int radius);
    private:
        SolidObjGameEngine* gameEngine;
};

//SolidObjGameEngine: contains the collision logic for SolidObject
class SolidObjGameEngine
{
    public:
        SolidObjGameEngine()
        {
            AllocatedCollisionSpaces = 0;
        }

        CollisionSpaceHandle_t createCollisionSpace(olc::vi2d* collisionSurface, uint32_t size);

        void registerCollision(CollisionSpaceHandle_t collisionSpace,SolidObject* ObjectPtr);

    private:
        std::uint32_t AllocatedCollisionSpaces;
        std::vector<CollisionSurface_t*> CollisionMatrix;
};