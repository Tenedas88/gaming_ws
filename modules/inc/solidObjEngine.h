#include "olcPixelGameEngine.h"
#include <vector>

#define INVALID_COLLISION_HANDLE        (UINT32_MAX)
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
    uint32_t                   registeredCollisions; //dimension of the collisionsArrayObject
    CollisionVector_t          collisionsArray;
} CollisionSurface_t;

//SolidObject is the base object of the SolidObjGameEngine
class SolidObject
{
    public:
        SolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius);

        //~SolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius){//TODO}

        //getter-setter for collision space
        void setCollisionSpace(CollisionSpaceHandle_t inputCollisionSpace){this->collisionSpace = inputCollisionSpace;}
        CollisionSpaceHandle_t getCollisionSpace(void){return this->collisionSpace;}
        
        olc::vi2d updatePosition(olc::vi2d& destination, void* Args);

        olc::vi2d getPosition(void){return this->position;}
        unsigned int getRadius(void){return this->radius;}

        virtual olc::vi2d getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination)=0;
        
    protected:
        
        virtual olc::vi2d objectMovementFunction(void* Args){(void)Args;return this->position;};
        
        olc::vi2d              position;
        unsigned int           radius;
        SolidObjGameEngine*    gameEngine;
        CollisionSpaceHandle_t collisionSpace;
};

//class for round objects
class SolidRoundObject : public SolidObject
{
    public:
        SolidRoundObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
            SolidObject(solidEngine,onCreatePosition,onCreateRadius){}

        virtual olc::vi2d getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination);
};

//Class for polygons objects
class SolidPolygonObject : public SolidObject
{
    public:
        SolidPolygonObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius,
                            CollisionSurfaceCorners_t& corners):
            SolidObject(solidEngine,onCreatePosition,onCreateRadius)
            {
                for(auto corner = corners.begin();corner <corners.end();corner++)
                {
                    this->surfacecorners.push_back((*corner)+onCreatePosition);
                    this->surfaceSize++;
                }
            }

        virtual olc::vi2d getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination);

    protected:
        //constructor for hineritance
        SolidPolygonObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
            SolidObject(solidEngine,onCreatePosition,onCreateRadius){}

        uint32_t                  surfaceSize;
        CollisionSurfaceCorners_t surfacecorners;
};

//class for round objects
class SolidLineObject : public SolidPolygonObject
{
    public:
        SolidLineObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,olc::vi2d& onCreateDirection,unsigned int onCreateRadius):
            SolidPolygonObject(solidEngine,onCreatePosition,onCreateRadius)
            {
                this->surfaceSize = 2;
                this->surfacecorners.push_back(onCreatePosition+(onCreateDirection*(onCreateRadius/2)));
                this->surfacecorners.push_back(onCreatePosition-(onCreateDirection*(onCreateRadius/2)));
            }

        virtual olc::vi2d getAllowedDestination(SolidObject* targetObject, olc::vi2d targetDestination) override;
};

//Class for square objects
class SolidSquareObject : public SolidPolygonObject
{
    public:
        SolidSquareObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
            SolidPolygonObject(solidEngine,onCreatePosition,onCreateRadius)
            {
                this->surfaceSize = 4;
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(-onCreateRadius,-onCreateRadius));
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(onCreateRadius,-onCreateRadius));
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(onCreateRadius,onCreateRadius));
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(-onCreateRadius,onCreateRadius));
            }
};

//Class for square objects
class SolidTriangleObject : public SolidPolygonObject
{
    public:
        SolidTriangleObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
            SolidPolygonObject(solidEngine,onCreatePosition,onCreateRadius)
            {
                this->surfaceSize = 3;
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(0,-onCreateRadius));
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d((onCreateRadius/2)*2,onCreateRadius/3));
                this->surfacecorners.push_back(onCreatePosition+olc::vi2d(-(onCreateRadius/2)*2,onCreateRadius/3));
            }
};

//SolidObjGameEngine: contains the collision logic for SolidObject
class SolidObjGameEngine
{
    public:
        SolidObjGameEngine():collisionMatrix()
        {
            allocatedCollisionSpaces = 0;
        }

        CollisionSpaceHandle_t createCollisionSpace(olc::vi2d* collisionSurface, uint32_t size);

        void registerSolidObject(CollisionSpaceHandle_t collisionSpace,SolidObject* objectPtr);

        olc::vi2d calculateSolidDestination(SolidObject* targetObject, olc::vi2d targetDestination);

    private:
        std::uint32_t                    allocatedCollisionSpaces;
        std::vector<CollisionSurface_t*> collisionMatrix;
};