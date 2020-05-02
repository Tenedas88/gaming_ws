#include "olcPixelGameEngine.h"
#include "solidObjEngine.h"

class MySolidObject : public SolidObject
{
	public:
		MySolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
			SolidObject(solidEngine,onCreatePosition,onCreateRadius),
			lastPosition(onCreatePosition)
		{
			reset = false;
		}

	private:
		virtual olc::vi2d objectMovementFunction(void* Args) override
		{
			olc::vi2d destination = this->getPosition();
			if(Args != NULL)
			{
				olc::vi2d* inputDestination = (olc::vi2d*)Args;
				if(abs(inputDestination->y-lastPosition.y) < this->getRadius() && reset)
				{
					destination.y = 175+rand()%25;
					reset = false;
				}
				else
				{
					(inputDestination->y < destination.y) ? destination.y-=1:destination.y +=1;
					destination.y = destination.y%240;
					lastPosition = destination;
					reset = true;
				}
			}

			return destination;
		}

		olc::vi2d lastPosition;
		bool      reset;
};

class Example : public olc::PixelGameEngine
{
public:
	SolidObjGameEngine*    SolidEngine = NULL;
	olc::vi2d 			   stoppedObjStart;
	olc::vi2d 			   stoppedObjLast;
	MySolidObject*         stoppedObj  = NULL;
	olc::vi2d			   movingObjStart;
	olc::vi2d			   movingObjLast;
	MySolidObject*         movingObj   = NULL;

	olc::vi2d 			   destination;
	CollisionSpaceHandle_t objSpace    = INVALID_COLLISION_HANDLE;
	olc::vi2d 			   surface     = {0,0};//TODO: add support for surface shape calculation

	Example():stoppedObjStart(120,200),movingObjStart(120,128),destination(120,240)
	{
		this->SolidEngine = new SolidObjGameEngine();
		this->stoppedObj  = new MySolidObject(SolidEngine,stoppedObjStart,4);
		this->movingObj   = new MySolidObject(SolidEngine,movingObjStart,3);

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
				Draw(x, y, olc::BLANK);	

		DrawCircle(stoppedObjStart,2,olc::RED);
		DrawCircle(movingObjStart,2,olc::WHITE);
		DrawCircle(stoppedObj->updatePosition(movingObjStart,(void*)&movingObjStart),stoppedObj->getRadius(),olc::WHITE);
		DrawCircle(movingObj->updatePosition(stoppedObjStart,(void*)&stoppedObjStart),movingObj->getRadius(),olc::RED);

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
