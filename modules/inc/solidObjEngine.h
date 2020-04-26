#include "olcPixelGameEngine.h"

class SolidObject;

typedef olc::vi2d (SolidObject::*checkCollision_t)(olc::vi2d& destination, unsigned int radius);

class SolidObject 
{
    olc::vi2d checkCollision(olc::vi2d& destination, unsigned int radius);
};

class SolidObjGameEngine : public olc::PixelGameEngine
{
    public:
        void registerSolidObject(checkCollision_t checkCollisionFunction, unsigned int radius);
    private:
};