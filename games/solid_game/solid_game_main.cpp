#include "olcPixelGameEngine.h"
#include "solidObjEngine.h"

typedef bool MySolidObjectContext_t;

class MySolidObject : public SolidObject
{
	public:
		MySolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
			SolidObject(solidEngine,onCreatePosition,onCreateRadius)
		{

		}

	private:
		virtual olc::vi2d objectMovementFunction(void* Args) override
		{
			MySolidObjectContext_t Ctx = (MySolidObjectContext_t*)Args;
			olc::vi2d destination = this->getPosition();
			if(Ctx)
			{
				destination.x += 1%240;
			}

			return destination;
		}
};

class Example : public olc::PixelGameEngine
{
public:
	SolidObjGameEngine*    SolidEngine = NULL;
	olc::vi2d 			   stoppedObjStart;
	MySolidObject*         stoppedObj  = NULL;
	olc::vi2d			   movingObjStart;
	MySolidObject*         movingObj   = NULL;

	olc::vi2d 			   destination;
	CollisionSpaceHandle_t objSpace    = INVALID_COLLISION_HANDLE;
	olc::vi2d 			   surface     = {0,0};//TODO: add support for surface shape calculation

	Example():stoppedObjStart(120,150),movingObjStart(120,128),destination(120,240)
	{
		SolidObjGameEngine* SolidEngine = new SolidObjGameEngine();
		MySolidObject*      stoppedObj  = new MySolidObject(SolidEngine,stoppedObjStart,10);
		MySolidObject*      movingObj   = new MySolidObject(SolidEngine,movingObjStart,10);

		objSpace = SolidEngine->createCollisionSpace(&surface, 1);
		SolidEngine->registerSolidObject(objSpace,stoppedObj);
		SolidEngine->registerSolidObject(objSpace,movingObj);
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				//Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
				movingObj->updatePosition(destination,(void*)true);

				DrawCircle(movingObj->getPosition(),movingObj->getRadius(),olc::RED);
				DrawCircle(stoppedObj->getPosition(),stoppedObj->getRadius(),olc::RED);
		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
